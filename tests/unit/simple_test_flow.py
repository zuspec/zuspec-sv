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
            if line.startswith(pref):
                ret.append(line)
    return ret

def clean_expect(expect):
    ret = []
    for line in io.StringIO(expect).readlines():
        line = line.strip()
        if line != "":
            ret.append(line)
    return ret
    

def run_unit_test(dirconfig, content, expect):
    top_pss = dirconfig.mkBuildDirFile("top.pss", content)
    actor_sv = os.path.join(dirconfig.builddir(), "actor.sv")
    zsp_sv = os.path.abspath(os.path.join(
        dirconfig.test_srcdir(),
        "../../src/include/zsp/sv/zsp_sv"
    ))
#    flow = pfv.FlowSim(dirconfig, sim_id="mti")
    flow = pfv.FlowSim(dirconfig, sim_id="vlt")

    print("test_srcdir: %s" % dirconfig.test_srcdir(), flush=True)
    flow.addTaskToPhase("generate.main", TaskGenSvActor(
        "pss_top", 
        "pss_top::Entry", 
        actor_sv,
        [pfv.FSPaths([top_pss], "pssSource")]))
    flow.addFileset("sim",
        pfv.FSPaths(
            [
                os.path.join(zsp_sv, "zsp_sv.sv"), 
                actor_sv],
            "systemVerilogSource",
            incs=[zsp_sv]
        )
    )
    flow.addFileset("sim",
        pfv.FSPaths(
            [os.path.join(dirconfig.test_srcdir(), "data", "top.sv")],
            "systemVerilogSource"
        )
    )

    flow.sim.top.add("top")

    run_args = flow.sim.mkRunArgs(dirconfig.rundir())
    flow.addTaskToPhase("run.main", flow.sim.mkRunTask(run_args))
    async def check(args, expect):
        runlog_l = read_runlog(os.path.join(args.rundir, args.run_logfile), "RES:")
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