import os
import dataclasses as dc
import pytest
import shutil
import pytest_dfm

def _available_sims():
    sims = []
    for sim,exe in [
        ('mti', 'vsim'), 
        ('vcs', 'vcs'),
        ('vlt', 'verilator'),
        ('xcm', 'xmvlog'),
        ('xsm', 'xvlog')]:
        if shutil.which(exe) is not None:
            sims.append(sim)

    return sims

@dc.dataclass
class SimDvFlow(pytest_dfm.DvFlow):
    sim: str = ""

    def __post_init__(self):
        super().__post_init__()
        self.addOverride("hdlsim", "hdlsim.%s" % self.sim)


@pytest.fixture(scope='function', params=_available_sims())
def sim_dvflow(request, tmpdir):
    dvflow = SimDvFlow(
        request, 
        os.path.dirname(request.fspath), 
        tmpdir,
        sim=request.param)
    return dvflow

