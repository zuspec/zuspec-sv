cimport ciostream.core as ciostream
cimport zsp_arl_dm.decl as arl_dm
cimport zsp_arl_eval.decl as arl_eval
cimport vsc_dm.decl as vsc_dm_decl
cimport debug_mgr.decl as dm

from libcpp.string cimport string as cpp_string
from libcpp.vector cimport vector as cpp_vector
from libcpp.memory cimport unique_ptr
from libcpp cimport bool
from libc.stdint cimport int32_t
cimport cpython.ref as cpy_ref

ctypedef IFactory *IFactoryP
ctypedef ITaskGenerate *ITaskGenerateP

cdef extern from "zsp/sv/IFactory.h" namespace "zsp::sv":
    cdef cppclass IFactory:
        void init(dm.IDebugMgr *)
        ITaskGenerate *mkGenerateExecActor(
            arl_dm.IContext             *ctxt,
            arl_eval.IFactory           *eval_f,
            arl_dm.IDataTypeComponent   *comp_t,
            arl_dm.IDataTypeAction      *action_t,
            ciostream.ostream           *out
        )

cdef extern from "zsp/sv/gen/ITaskGenerate.h" namespace "zsp::sv::gen":
    cdef cppclass ITaskGenerate:
        bool generate()