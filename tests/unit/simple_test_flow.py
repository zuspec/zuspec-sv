import os
import io
import sys
import pytest_fv as pfv
import pytest_dv as pdv
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
        dvflow, 
        content, 
        expect,
        test_top="top.sv",
        prefixes=("RES:",),
        debug=False,
        extra_content=None):

    unit_test_dir = os.path.dirname(os.path.abspath(__file__))
    dvflow.loadPkg(os.path.join(unit_test_dir, "flow.dv"))
#    dvflow.addOverride("hdlsim", "hdlsim.vlt")

    # TODO: Register new 


#    actor_sv = os.path.join(dirconfig.builddir(), "actor.sv")
    zsp_sv_dir = os.path.abspath(os.path.join(
        dvflow.srcdir, "../../src/include/zsp/sv/zsp_sv"))
    
    if not isinstance(prefixes,tuple):
        prefixes = (prefixes,)

    sv_sources = []

    zsp_sv = dvflow.mkTask(
        "std.FileSet", name="zsp_sv", type="systemVerilogSource", 
        base=zsp_sv_dir, include=["zsp_sv.sv"])

    top_pss = dvflow.mkTask("std.CreateFile", name="top_pss", type="pssSource", 
                            filename="top.pss", content=content, needs=[zsp_sv])

    actor_sv = dvflow.mkTask(
        "zsp.GenSvActor", "gen_actor", needs=[zsp_sv, top_pss],
        action="pss_top::Entry")

    extra_content_tasks = []
    if extra_content is not None:
        for path,content in extra_content.items():
            extra_content_tasks.append(dvflow.mkTask("std.CreateFile", 
                        name="path", filename=path, content=content,
                        needs=[zsp_sv, actor_sv]))

    top_sv_needs = extra_content_tasks.copy()
    top_sv_needs.append(zsp_sv)
    top_sv_needs.append(actor_sv)
    top_sv = dvflow.mkTask("std.FileSet", 
        name="top_sv", type="systemVerilogSource", 
        base=os.path.join(dvflow.srcdir, "data"), 
        include=test_top, needs=top_sv_needs)

    sim_img = dvflow.mkTask('hdlsim.SimImage', name='sim_img',
                        needs=[top_sv],
                        top=['top'])
    sim_run = dvflow.mkTask('hdlsim.SimRun', name='sim_run',
                        needs=[sim_img])
    
    status, out = dvflow.runTask(sim_run)

    assert status == 0

    runlog_l = read_runlog(os.path.join(out.output[0].basedir, "sim.log"), prefixes)
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
