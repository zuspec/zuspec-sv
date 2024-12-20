// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource__Syms.h"
#include "Vresource_resource_pkg__03a__03aA_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aB_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aC_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

Vresource_resource_pkg__03a__03aresource::Vresource_resource_pkg__03a__03aresource(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::new\n"); );
    // Init
    _ctor_var_reset(vlSymsp);
    // Body
    this->__PVT__C = VL_NEW(Vresource_resource_pkg__03a__03aC_data, vlSymsp);
    this->__PVT__B = VL_NEW(Vresource_resource_pkg__03a__03aB_data, vlSymsp);
    this->__PVT__A = VL_NEW(Vresource_resource_pkg__03a__03aA_data, vlSymsp);
}
