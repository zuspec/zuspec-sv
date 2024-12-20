// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VRESOURCE__SYMS_H_
#define VERILATED_VRESOURCE__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vresource.h"

// INCLUDE MODULE CLASSES
#include "Vresource___024root.h"
#include "Vresource_resource_pkg.h"
#include "Vresource_resource_pkg__03a__03aA_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aB_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aC_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vresource__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vresource* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vresource___024root            TOP;
    Vresource_resource_pkg         TOP__resource_pkg;
    Vresource_resource_pkg__03a__03aA_data__Vclpkg TOP__resource_pkg__03a__03aA_data__Vclpkg;
    Vresource_resource_pkg__03a__03aB_data__Vclpkg TOP__resource_pkg__03a__03aB_data__Vclpkg;
    Vresource_resource_pkg__03a__03aC_data__Vclpkg TOP__resource_pkg__03a__03aC_data__Vclpkg;
    Vresource_resource_pkg__03a__03aresource__Vclpkg TOP__resource_pkg__03a__03aresource__Vclpkg;

    // CONSTRUCTORS
    Vresource__Syms(VerilatedContext* contextp, const char* namep, Vresource* modelp);
    ~Vresource__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
