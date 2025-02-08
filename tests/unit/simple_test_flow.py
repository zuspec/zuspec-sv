import os
import io
import sys
import pytest_fv as pfv
from typing import List

try:
    from zsp_sv.pytest_fv import TaskGenSvActor
except ModuleNotFoundError:
    sys.path.insert(0,
        os.path.join(
            os.path.dirname(
                os.path.dirname(
                    os.path.dirname(os.path.abspath(__file__)))), "python"))
    print(sys.path)
    from zsp_sv.pytest_fv import TaskGenSvActor

def read_runlog(runlog, pref) -> List[str]:
    ret = []
    with open(runlog, "r") as fp:
        for line in fp.readlines():
            if line.startswith("#"):
                line = line[1:]
            line = line.strip()
            for p in pref:
                if line.startswith(p):
                    ret.append(line)
    return ret

def clean_expect(expect):
    ret = []
    for line in io.StringIO(expect).readlines():
        line = line.strip()
        if line != "":
            ret.append(line)
    return ret
    

def run_unit_test(
        dirconfig, 
        content, 
        expect,
        test_top="top.sv",
        prefixes=("RES:",),
        debug=False,
        extra_content=None):
    top_pss = dirconfig.mkBuildDirFile("top.pss", content)
    actor_sv = os.path.join(dirconfig.builddir(), "actor.sv")
    zsp_sv = os.path.abspath(os.path.join(
        dirconfig.test_srcdir(),
        "../../src/include/zsp/sv/zsp_sv"
    ))
#    flow = pfv.FlowSim(dirconfig, sim_id="mti")
#    flow = pfv.FlowSim(dirconfig, sim_id="vlt")
    flow = pfv.FlowSim(dirconfig)

    if not isinstance(prefixes,tuple):
        prefixes = (prefixes,)

    if extra_content is not None:
        for path,content in extra_content.items():
            full_path = os.path.join(dirconfig.builddir(), path)
            if not os.path.exists(os.path.dirname(full_path)):
                os.makedirs(os.path.dirname(full_path))
            with open(full_path, "w") as fp:
                fp.write(content)

    print("test_srcdir: %s" % dirconfig.test_srcdir(), flush=True)
    flow.addTaskToPhase("generate.main", TaskGenSvActor(
        "pss_top", 
        "pss_top::Entry", 
        actor_sv,
        [pfv.FSPaths(
            os.path.dirname(top_pss),
            [top_pss], 
            "pssSource")],
        debug=debug))
    flow.addFileset("sim",
        pfv.FSPaths(
            zsp_sv,
            ["zsp_sv.sv"], 
            "systemVerilogSource",
            incs=[
                zsp_sv,
                dirconfig.builddir()]
        )
    )
    flow.addFileset("sim",
        pfv.FSPaths(
            os.path.dirname(actor_sv),
            [actor_sv],
            "systemVerilogSource"
        )
    )
    flow.addFileset("sim",
        pfv.FSPaths(
            dirconfig.test_srcdir(),
            [os.path.join("data", test_top)],
            "systemVerilogSource"
        )
    )

    flow.sim.top.add("top")

    run_args = flow.sim.mkRunArgs(dirconfig.rundir())
    flow.addTaskToPhase("run.main", flow.sim.mkRunTask(run_args))
    async def check(args, expect):
        runlog_l = read_runlog(os.path.join(args.rundir, args.run_logfile), prefixes)
        expect_l = clean_expect(expect)
        errors = 0
        for i in range(max(len(runlog_l),len(expect_l))):
            if i < len(runlog_l) and i < len(expect_l):
                if runlog_l[i] == expect_l[i]:
                    print("OK: %s" % runlog_l[i])
                else:
                    print("EXP: %s" % expect_l[i])
                    print("ACT: %s" % runlog_l[i])
                    errors += 1
            else:
                if i < len(runlog_l):
                    print("ACT: %s" % runlog_l[i])
                if i < len(expect_l):
                    print("ACT: %s" % expect_l[i])
                errors += 1

        assert errors == 0

    flow.addTaskToPhase("run.post", check(run_args, expect))
    flow.run_all()