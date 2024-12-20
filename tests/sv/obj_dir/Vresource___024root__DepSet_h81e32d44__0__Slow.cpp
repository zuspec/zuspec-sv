// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource___024root.h"

VL_ATTR_COLD void Vresource___024root___eval_static(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_static\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vresource___024root___eval_initial__TOP(Vresource___024root* vlSelf);

VL_ATTR_COLD void Vresource___024root___eval_initial(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_initial\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vresource___024root___eval_initial__TOP(vlSelf);
}

VL_ATTR_COLD void Vresource___024root___eval_final(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_final\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vresource___024root___eval_settle(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_settle\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vresource___024root___dump_triggers__act(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___dump_triggers__act\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vresource___024root___dump_triggers__nba(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___dump_triggers__nba\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vresource___024root___ctor_var_reset(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___ctor_var_reset\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
}
