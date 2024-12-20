
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

    cpdef void sayHello(self)

    cpdef void prepContextExec(
        self,
        arl_dm.Context             ctxt)

    cpdef TaskGenerate mkGenerateActorPkg(
        self,
        arl_dm.Context             ctxt,
        arl_dm.DataTypeComponent   comp_t,
        arl_dm.DataTypeAction      action_t,
        object                     out)

    cpdef TaskGenerate mkGenerateActorPkgPrv(
        self,
        arl_dm.Context             ctxt,
        arl_dm.DataTypeComponent   comp_t,
        arl_dm.DataTypeAction      action_t,
        object                     out)

    cpdef TaskGenerate mkGenerateTypesPkg(
        self,
        arl_dm.Context             ctxt,
        object                     out)

cdef class TaskGenerate(object):
    cdef decl.ITaskGenerate     *_hndl
    cdef bool                   _owned
    cdef costream               _out

    cpdef bool generate(self)

    @staticmethod
    cdef TaskGenerate mk(decl.ITaskGenerate *hndl, costream out, bool owned=*)


