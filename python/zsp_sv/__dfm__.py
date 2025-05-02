
import os
from zsp_sv import get_incdirs

def dvfm_packages():
    zsp_dir = os.path.dirname(os.path.abspath(__file__))
    incdirs = get_incdirs()
    zsp_rt_dir = os.path.join(incdirs[0], "zsp/sv/zsp_sv")


    return {
        'zsp.sv': os.path.join(zsp_dir, "flow.dv"),
        'zsp.sv.rt': os.path.join(zsp_rt_dir, "flow.dv")
    }