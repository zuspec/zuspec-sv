import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
from zsp_sv.pytest_fv import TaskGenSvActor

class runnable(object):

    async def run(self):
        print("run")

def test_bool_field(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    print("Hello World!");
                }
            }
        }
    """
    top_pss = dirconfig.mkBuildDirFile("top.pss", content)
    actor_sv = os.path.join(dirconfig.builddir(), "actor.sv")
    zsp_sv = os.path.abspath(os.path.join(
        dirconfig.test_srcdir(),
        "../../src/include/zsp/sv/zsp_sv"
    ))
    flow = pfv.FlowSim(dirconfig, sim_id="mti")

    print("test_srcdir: %s" % dirconfig.test_srcdir(), flush=True)
    flow.addTaskToPhase("generate.main", TaskGenSvActor(
        "pss_top", 
        "pss_top::Entry", 
        actor_sv,
        [pfv.FSPaths([top_pss], "pssSource")]))
    flow.addFileset("sim",
        pfv.FSPaths(
            [actor_sv],
            "systemVerilogSource"
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
    flow.run_all()

    pass