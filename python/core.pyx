
import os
import ctypes
import typing
from zsp_sv cimport decl
from libcpp.vector cimport vector as cpp_vector
from libc.stdint cimport intptr_t
from ciostream.core import costream
cimport debug_mgr.core as dm_core
cimport vsc_dm.decl as vsc_dm_decl
cimport vsc_dm.core as vsc_dm
cimport zsp_arl_dm.core as arl_dm
cimport zsp_arl_dm.decl as arl_dm_decl
cimport zsp_arl_eval.core as arl_eval

cdef _FactoryInst = None

cdef class Factory(object):

    cpdef void init(self, dm_core.Factory dmgr):
        self._hndl.init(dmgr._hndl.getDebugMgr())

    cpdef void sayHello(self):
        print("Hello from Factory")

    cpdef void prepContextExec(
        self,
        arl_dm.Context             ctxt):
        print("Hello from prepContextExec")
#        cdef arl_eval.Factory eval_f = arl_eval.Factory.inst()
#        self._hndl.prepContextExec(
#            ctxt.asContext(),
#            eval_f._hndl)
        pass

    cpdef TaskGenerate mkGenerateActorPkg(
        self,
        arl_dm.Context             ctxt,
        arl_dm.DataTypeComponent   comp_t,
        arl_dm.DataTypeAction      action_t,
        object                     out):
        cdef arl_eval.Factory eval_f = arl_eval.Factory.inst()
        cdef costream out_s = costream(out)
        cdef decl.ITaskGenerate *gen = self._hndl.mkGenerateActorPkg(
            ctxt.asContext(),
            eval_f._hndl,
            comp_t.asComponent(),
            action_t.asAction(),
            out_s.stream())
        return TaskGenerate.mk(gen, out_s, True)

    cpdef TaskGenerate mkGenerateActorPkgPrv(
        self,
        arl_dm.Context             ctxt,
        arl_dm.DataTypeComponent   comp_t,
        arl_dm.DataTypeAction      action_t,
        object                     out):
        cdef arl_eval.Factory eval_f = arl_eval.Factory.inst()
        cdef costream out_s = costream(out)
        cdef decl.ITaskGenerate *gen = self._hndl.mkGenerateActorPkgPrv(
            ctxt.asContext(),
            eval_f._hndl,
            comp_t.asComponent(),
            action_t.asAction(),
            out_s.stream())
        return TaskGenerate.mk(gen, out_s, True)

    cpdef TaskGenerate mkGenerateTypesPkg(
        self,
        arl_dm.Context             ctxt,
        object                     out):
        cdef arl_eval.Factory eval_f = arl_eval.Factory.inst()
        cdef costream out_s = costream(out)
        cdef decl.ITaskGenerate *gen = self._hndl.mkGenerateTypesPkg(
            ctxt.asContext(),
            eval_f._hndl,
            out_s.stream())
        return TaskGenerate.mk(gen, out_s, True)

    @staticmethod
    def inst():
        cdef Factory factory
        global _FactoryInst

        if _FactoryInst is None:
            ext_dir = os.path.dirname(os.path.abspath(__file__))
            build_dir = os.path.join(
                os.path.dirname(os.path.dirname(ext_dir)), "build")

            core_lib = None
            if os.path.isdir(build_dir):
                for libdir in ("lib", "lib64"):
                    core_lib = os.path.join(build_dir, libdir, "libzsp-sv.so")
                    if os.path.isfile(core_lib):
                        break
                    else:
                        core_lib = None
            if core_lib is None:
                core_lib = os.path.join(ext_dir, "libzsp-sv.so")
            if not os.path.isfile(core_lib):
                raise Exception("Extension library core \"%s\" doesn't exist" % core_lib)
            so = ctypes.cdll.LoadLibrary(core_lib)

            func = so.zsp_sv_getFactory
            func.restype = ctypes.c_void_p

            hndl = <decl.IFactoryP>(<intptr_t>(func()))
            factory = Factory()
            factory._hndl = hndl
            factory.init(dm_core.Factory.inst())
            _FactoryInst = factory
        return _FactoryInst

cdef class TaskGenerate(object):

    cpdef bool generate(self):
        return self._hndl.generate()

    @staticmethod
    cdef TaskGenerate mk(decl.ITaskGenerate *hndl, costream out, bool owned=True):
        ret = TaskGenerate()
        ret._hndl = hndl
        ret._out = out
        ret._owned = owned
        return ret

