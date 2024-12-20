// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vresource.h for the primary calling header

#ifndef VERILATED_VRESOURCE___024ROOT_H_
#define VERILATED_VRESOURCE___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_random.h"
class Vresource_resource_pkg;
class Vresource_resource_pkg__03a__03aA_data__Vclpkg;
class Vresource_resource_pkg__03a__03aB_data__Vclpkg;
class Vresource_resource_pkg__03a__03aC_data__Vclpkg;
class Vresource_resource_pkg__03a__03aresource;
class Vresource_resource_pkg__03a__03aresource__Vclpkg;


class Vresource__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vresource___024root final : public VerilatedModule {
  public:
    // CELLS
    Vresource_resource_pkg* __PVT__resource_pkg;
    Vresource_resource_pkg__03a__03aA_data__Vclpkg* resource_pkg__03a__03aA_data__Vclpkg;
    Vresource_resource_pkg__03a__03aB_data__Vclpkg* resource_pkg__03a__03aB_data__Vclpkg;
    Vresource_resource_pkg__03a__03aC_data__Vclpkg* resource_pkg__03a__03aC_data__Vclpkg;
    Vresource_resource_pkg__03a__03aresource__Vclpkg* resource_pkg__03a__03aresource__Vclpkg;

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vresource__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vresource___024root(Vresource__Syms* symsp, const char* v__name);
    ~Vresource___024root();
    VL_UNCOPYABLE(Vresource___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
