// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource__Syms.h"
#include "Vresource___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vresource___024root___dump_triggers__act(Vresource___024root* vlSelf);
#endif  // VL_DEBUG

void Vresource___024root___eval_triggers__act(Vresource___024root* vlSelf) {
    (void)vlSelf;  // Prevent unused variable warning
    Vresource__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vresource___024root___eval_triggers__act\n"); );
    auto &vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vresource___024root___dump_triggers__act(vlSelf);
    }
#endif
}
