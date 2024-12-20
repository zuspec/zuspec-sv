// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vresource__pch.h"

//============================================================
// Constructors

Vresource::Vresource(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vresource__Syms(contextp(), _vcname__, this)}
    , __PVT__resource_pkg{vlSymsp->TOP.__PVT__resource_pkg}
    , resource_pkg__03a__03aA_data__Vclpkg{vlSymsp->TOP.resource_pkg__03a__03aA_data__Vclpkg}
    , resource_pkg__03a__03aB_data__Vclpkg{vlSymsp->TOP.resource_pkg__03a__03aB_data__Vclpkg}
    , resource_pkg__03a__03aC_data__Vclpkg{vlSymsp->TOP.resource_pkg__03a__03aC_data__Vclpkg}
    , resource_pkg__03a__03aresource__Vclpkg{vlSymsp->TOP.resource_pkg__03a__03aresource__Vclpkg}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vresource::Vresource(const char* _vcname__)
    : Vresource(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vresource::~Vresource() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vresource___024root___eval_debug_assertions(Vresource___024root* vlSelf);
#endif  // VL_DEBUG
void Vresource___024root___eval_static(Vresource___024root* vlSelf);
void Vresource___024root___eval_initial(Vresource___024root* vlSelf);
void Vresource___024root___eval_settle(Vresource___024root* vlSelf);
void Vresource___024root___eval(Vresource___024root* vlSelf);

void Vresource::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vresource::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vresource___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vresource___024root___eval_static(&(vlSymsp->TOP));
        Vresource___024root___eval_initial(&(vlSymsp->TOP));
        Vresource___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vresource___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vresource::eventsPending() { return false; }

uint64_t Vresource::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vresource::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vresource___024root___eval_final(Vresource___024root* vlSelf);

VL_ATTR_COLD void Vresource::final() {
    Vresource___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vresource::hierName() const { return vlSymsp->name(); }
const char* Vresource::modelName() const { return "Vresource"; }
unsigned Vresource::threads() const { return 1; }
void Vresource::prepareClone() const { contextp()->prepareClone(); }
void Vresource::atClone() const {
    contextp()->threadPoolpOnClone();
}
