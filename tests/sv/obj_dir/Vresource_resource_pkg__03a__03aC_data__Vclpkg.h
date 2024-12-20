// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vresource.h for the primary calling header

#ifndef VERILATED_VRESOURCE_RESOURCE_PKG__03A__03AC_DATA__VCLPKG_H_
#define VERILATED_VRESOURCE_RESOURCE_PKG__03A__03AC_DATA__VCLPKG_H_  // guard

#include "verilated.h"
#include "verilated_random.h"


class Vresource__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vresource_resource_pkg__03a__03aC_data__Vclpkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vresource__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vresource_resource_pkg__03a__03aC_data__Vclpkg(Vresource__Syms* symsp, const char* v__name);
    ~Vresource_resource_pkg__03a__03aC_data__Vclpkg();
    VL_UNCOPYABLE(Vresource_resource_pkg__03a__03aC_data__Vclpkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


class Vresource__Syms;

class Vresource_resource_pkg__03a__03aC_data : public VlClass {
  public:

    // DESIGN SPECIFIC STATE
    CData/*7:0*/ __PVT__comp_id;
    CData/*7:0*/ __PVT__pcomp_id;
    CData/*7:0*/ __PVT__r_pool_id;
    CData/*7:0*/ __PVT__r_inst_id;
    VlRandomizer __PVT__constraint;
    void __VnoInFunc_print(Vresource__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset(Vresource__Syms* __restrict vlSymsp);
  public:
    Vresource_resource_pkg__03a__03aC_data(Vresource__Syms* __restrict vlSymsp);
    std::string to_string() const;
    std::string to_string_middle() const;
    ~Vresource_resource_pkg__03a__03aC_data();
};

std::string VL_TO_STRING(const VlClassRef<Vresource_resource_pkg__03a__03aC_data>& obj);

#endif  // guard
