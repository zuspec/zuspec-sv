
import ctypes
from zsp_sv cimport decl
cimport vsc_dm.core as vsc_dm
cimport zsp_arl_dm.core as arl_dm
cimport debug_mgr.core as dm_core
cimport ciostream.core as ciostream
from libcpp cimport bool
from libc.stdint cimport int32_t

cdef class Factory(object):
    cdef decl.IFactory      *_hndl

    cpdef void init(self, dm_core.Factory dmgr)


cdef class TaskGenerate(object):
    cdef decl.ITaskGenerate     *_hndl
    cdef bool                   _owned

    cpdef bool generate(self)

    @staticmethod
    cdef TaskGenerate mk(decl.ITaskGenerate *hndl, bool owned=*)


