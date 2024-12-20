// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource__Syms.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

Vresource_resource_pkg__03a__03aresource::Vresource_resource_pkg__03a__03aresource(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
    // Body
    this->__PVT__constraint.write_var(this->__PVT__A_pcomp_id, 8ULL, 
                                      "A_pcomp_id");
    this->__PVT__constraint.write_var(this->__PVT__A_comp_id, 8ULL, 
                                      "A_comp_id");
    this->__PVT__constraint.write_var(this->__PVT__A_r_pool_id, 8ULL, 
                                      "A_r_pool_id");
    this->__PVT__constraint.write_var(this->__PVT__A_r_inst_id, 8ULL, 
                                      "A_r_inst_id");
    this->__PVT__constraint.write_var(this->__PVT__B_pcomp_id, 8ULL, 
                                      "B_pcomp_id");
    this->__PVT__constraint.write_var(this->__PVT__B_comp_id, 8ULL, 
                                      "B_comp_id");
    this->__PVT__constraint.write_var(this->__PVT__B_r_pool_id, 8ULL, 
                                      "B_r_pool_id");
    this->__PVT__constraint.write_var(this->__PVT__B_r_inst_id, 8ULL, 
                                      "B_r_inst_id");
    this->__PVT__constraint.write_var(this->__PVT__C_pcomp_id, 8ULL, 
                                      "C_pcomp_id");
    this->__PVT__constraint.write_var(this->__PVT__C_comp_id, 8ULL, 
                                      "C_comp_id");
    this->__PVT__constraint.write_var(this->__PVT__C_r_pool_id, 8ULL, 
                                      "C_r_pool_id");
    this->__PVT__constraint.write_var(this->__PVT__C_r_inst_id, 8ULL, 
                                      "C_r_inst_id");
}

void Vresource_resource_pkg__03a__03aresource::_ctor_var_reset(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::_ctor_var_reset\n"); );
    // Body
    (void)vlSymsp;  // Prevent unused variable warning
    __PVT__A_comp_id = 0;
    __PVT__A_pcomp_id = 0;
    __PVT__A_r_pool_id = 0;
    __PVT__A_r_inst_id = 0;
    __PVT__B_comp_id = 0;
    __PVT__B_pcomp_id = 0;
    __PVT__B_r_pool_id = 0;
    __PVT__B_r_inst_id = 0;
    __PVT__C_comp_id = 0;
    __PVT__C_pcomp_id = 0;
    __PVT__C_r_pool_id = 0;
    __PVT__C_r_inst_id = 0;
    }
