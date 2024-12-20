// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource_resource_pkg__03a__03aA_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aB_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aC_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_print(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_print\n"); );
    // Body
    VL_NULL_CHECK(this->__PVT__A, "resource.sv", 75)->__VnoInFunc_print(vlSymsp);
    VL_NULL_CHECK(this->__PVT__B, "resource.sv", 76)->__VnoInFunc_print(vlSymsp);
    VL_NULL_CHECK(this->__PVT__C, "resource.sv", 77)->__VnoInFunc_print(vlSymsp);
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_rsrc_c_setup_constraint(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_rsrc_c_setup_constraint\n"); );
    // Body
    this->__PVT__constraint.hard(VL_SFORMATF_N_NX("#b%b",0,
                                                  1,
                                                  (0U 
                                                   == VL_NULL_CHECK(this->__PVT__A, "resource.sv", 64)
                                                   ->__PVT__pcomp_id)) );
    this->__PVT__constraint.hard(VL_SFORMATF_N_NX("#b%b",0,
                                                  1,
                                                  (0U 
                                                   == VL_NULL_CHECK(this->__PVT__B, "resource.sv", 65)
                                                   ->__PVT__pcomp_id)) );
    this->__PVT__constraint.hard(VL_SFORMATF_N_NX("#b%b",0,
                                                  1,
                                                  (0U 
                                                   == VL_NULL_CHECK(this->__PVT__C, "resource.sv", 66)
                                                   ->__PVT__pcomp_id)) );
    this->__PVT__constraint.hard(VL_SFORMATF_N_NX("#b%b",0,
                                                  1,
                                                  (1U 
                                                   & (~ (IData)(
                                                                ((VL_NULL_CHECK(this->__PVT__A, "resource.sv", 69)
                                                                  ->__PVT__r_inst_id 
                                                                  == VL_NULL_CHECK(this->__PVT__B, "resource.sv", 69)
                                                                  ->__PVT__r_inst_id) 
                                                                 && (VL_NULL_CHECK(this->__PVT__A, "resource.sv", 69)
                                                                     ->__PVT__r_pool_id 
                                                                     == VL_NULL_CHECK(this->__PVT__B, "resource.sv", 69)
                                                                     ->__PVT__r_pool_id)))))) );
    this->__PVT__constraint.hard(VL_SFORMATF_N_NX("#b%b",0,
                                                  1,
                                                  (1U 
                                                   & (~ (IData)(
                                                                ((VL_NULL_CHECK(this->__PVT__A, "resource.sv", 70)
                                                                  ->__PVT__r_inst_id 
                                                                  == VL_NULL_CHECK(this->__PVT__C, "resource.sv", 70)
                                                                  ->__PVT__r_inst_id) 
                                                                 && (VL_NULL_CHECK(this->__PVT__A, "resource.sv", 70)
                                                                     ->__PVT__r_pool_id 
                                                                     == VL_NULL_CHECK(this->__PVT__C, "resource.sv", 70)
                                                                     ->__PVT__r_pool_id)))))) );
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vbasic_randomize(Vresource__Syms* __restrict vlSymsp, IData/*31:0*/ &__Vbasic_randomize__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vbasic_randomize\n"); );
    // Init
    IData/*31:0*/ __Vtask_randomize__9__Vfuncout;
    __Vtask_randomize__9__Vfuncout = 0;
    IData/*31:0*/ __Vtask_randomize__10__Vfuncout;
    __Vtask_randomize__10__Vfuncout = 0;
    IData/*31:0*/ __Vtask_randomize__11__Vfuncout;
    __Vtask_randomize__11__Vfuncout = 0;
    IData/*31:0*/ __Vtemp_1;
    IData/*31:0*/ __Vtemp_2;
    IData/*31:0*/ __Vtemp_3;
    // Body
    __Vbasic_randomize__Vfuncrtn = 1U;
    if ((VlNull{} != this->__PVT__A)) {
        __Vtemp_1 = (__Vbasic_randomize__Vfuncrtn & 
                     ([&]() {
                    VL_NULL_CHECK(this->__PVT__A, "resource.sv", 58)
                      ->__VnoInFunc_randomize(vlSymsp, __Vtask_randomize__9__Vfuncout);
                }(), __Vtask_randomize__9__Vfuncout));
        __Vbasic_randomize__Vfuncrtn = __Vtemp_1;
    }
    if ((VlNull{} != this->__PVT__B)) {
        __Vtemp_2 = (__Vbasic_randomize__Vfuncrtn & 
                     ([&]() {
                    VL_NULL_CHECK(this->__PVT__B, "resource.sv", 58)
                      ->__VnoInFunc_randomize(vlSymsp, __Vtask_randomize__10__Vfuncout);
                }(), __Vtask_randomize__10__Vfuncout));
        __Vbasic_randomize__Vfuncrtn = __Vtemp_2;
    }
    if ((VlNull{} != this->__PVT__C)) {
        __Vtemp_3 = (__Vbasic_randomize__Vfuncrtn & 
                     ([&]() {
                    VL_NULL_CHECK(this->__PVT__C, "resource.sv", 58)
                      ->__VnoInFunc_randomize(vlSymsp, __Vtask_randomize__11__Vfuncout);
                }(), __Vtask_randomize__11__Vfuncout));
        __Vbasic_randomize__Vfuncrtn = __Vtemp_3;
    }
}
