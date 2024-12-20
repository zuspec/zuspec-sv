// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource__Syms.h"
#include "Vresource___024root.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

VL_ATTR_COLD void Vresource___024root___eval_initial__TOP(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_initial__TOP\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlClassRef<Vresource_resource_pkg__03a__03aresource> top__DOT__unnamedblk1__DOT__r;
    IData/*31:0*/ __Vtask_randomize__1__Vfuncout;
    __Vtask_randomize__1__Vfuncout = 0;
    // Body
    top__DOT__unnamedblk1__DOT__r = VL_NEW(Vresource_resource_pkg__03a__03aresource, vlSymsp);
    if (VL_LIKELY((0U != ([&]() {
                        VL_NULL_CHECK(top__DOT__unnamedblk1__DOT__r, "resource.sv", 111)
                          ->__VnoInFunc_randomize(vlSymsp, __Vtask_randomize__1__Vfuncout);
                    }(), __Vtask_randomize__1__Vfuncout)))) {
        VL_NULL_CHECK(top__DOT__unnamedblk1__DOT__r, "resource.sv", 114)->__VnoInFunc_print(vlSymsp);
    } else {
        VL_WRITEF_NX("FAIL\n",0);
    }
    VL_FINISH_MT("resource.sv", 116, "");
}
