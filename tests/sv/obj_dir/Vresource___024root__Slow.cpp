// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource__Syms.h"
#include "Vresource___024root.h"

void Vresource___024root___ctor_var_reset(Vresource___024root* vlSelf);

Vresource___024root::Vresource___024root(Vresource__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vresource___024root___ctor_var_reset(this);
}

void Vresource___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vresource___024root::~Vresource___024root() {
}
