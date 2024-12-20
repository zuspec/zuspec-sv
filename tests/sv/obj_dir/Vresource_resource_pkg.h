// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vresource.h for the primary calling header

#ifndef VERILATED_VRESOURCE_RESOURCE_PKG_H_
#define VERILATED_VRESOURCE_RESOURCE_PKG_H_  // guard

#include "verilated.h"
#include "verilated_random.h"


class Vresource__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vresource_resource_pkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vresource__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vresource_resource_pkg(Vresource__Syms* symsp, const char* v__name);
    ~Vresource_resource_pkg();
    VL_UNCOPYABLE(Vresource_resource_pkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
