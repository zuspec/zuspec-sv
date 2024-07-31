
import ctypes
from zsp_sv cimport decl
cimport vsc_dm.core as vsc_dm
cimport zsp_arl_dm.core as arl_dm
cimport debug_mgr.core as dm_core
from ciostream.core cimport costream
from libcpp cimport bool
from libc.stdint cimport int32_t
from typing import IO

cdef class Factory(object):
    cdef decl.IFactory      *_hndl

    cpdef void init(self, dm_core.Factory dmgr)

    cpdef TaskGenerate mkGenerateExecActor(
        self,
        arl_dm.Context             ctxt,
        arl_dm.DataTypeComponent   comp_t,
        arl_dm.DataTypeAction      action_t,
        object                     out)

cdef class TaskGenerate(object):
    cdef decl.ITaskGenerate     *_hndl
    cdef bool                   _owned
    cdef costream               _out

    cpdef bool generate(self)

    @staticmethod
    cdef TaskGenerate mk(decl.ITaskGenerate *hndl, costream out, bool owned=*)


