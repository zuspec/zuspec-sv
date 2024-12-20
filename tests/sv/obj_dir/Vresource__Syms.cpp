// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vresource__pch.h"
#include "Vresource.h"
#include "Vresource___024root.h"
#include "Vresource_resource_pkg.h"
#include "Vresource_resource_pkg__03a__03aA_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aB_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aC_data__Vclpkg.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

// FUNCTIONS
Vresource__Syms::~Vresource__Syms()
{
}

Vresource__Syms::Vresource__Syms(VerilatedContext* contextp, const char* namep, Vresource* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
    , TOP__resource_pkg{this, Verilated::catName(namep, "resource_pkg")}
    , TOP__resource_pkg__03a__03aA_data__Vclpkg{this, Verilated::catName(namep, "resource_pkg::A_data__Vclpkg")}
    , TOP__resource_pkg__03a__03aB_data__Vclpkg{this, Verilated::catName(namep, "resource_pkg::B_data__Vclpkg")}
    , TOP__resource_pkg__03a__03aC_data__Vclpkg{this, Verilated::catName(namep, "resource_pkg::C_data__Vclpkg")}
    , TOP__resource_pkg__03a__03aresource__Vclpkg{this, Verilated::catName(namep, "resource_pkg::resource__Vclpkg")}
{
        // Check resources
        Verilated::stackCheck(31);
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    TOP.__PVT__resource_pkg = &TOP__resource_pkg;
    TOP.resource_pkg__03a__03aA_data__Vclpkg = &TOP__resource_pkg__03a__03aA_data__Vclpkg;
    TOP.resource_pkg__03a__03aB_data__Vclpkg = &TOP__resource_pkg__03a__03aB_data__Vclpkg;
    TOP.resource_pkg__03a__03aC_data__Vclpkg = &TOP__resource_pkg__03a__03aC_data__Vclpkg;
    TOP.resource_pkg__03a__03aresource__Vclpkg = &TOP__resource_pkg__03a__03aresource__Vclpkg;
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    TOP__resource_pkg.__Vconfigure(true);
    TOP__resource_pkg__03a__03aA_data__Vclpkg.__Vconfigure(true);
    TOP__resource_pkg__03a__03aB_data__Vclpkg.__Vconfigure(true);
    TOP__resource_pkg__03a__03aC_data__Vclpkg.__Vconfigure(true);
    TOP__resource_pkg__03a__03aresource__Vclpkg.__Vconfigure(true);
}
