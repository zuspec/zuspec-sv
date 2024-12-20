// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vresource.h for the primary calling header

#ifndef VERILATED_VRESOURCE_RESOURCE_PKG__03A__03ARESOURCE__VCLPKG_H_
#define VERILATED_VRESOURCE_RESOURCE_PKG__03A__03ARESOURCE__VCLPKG_H_  // guard

#include "verilated.h"
#include "verilated_random.h"


class Vresource__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vresource_resource_pkg__03a__03aresource__Vclpkg final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vresource__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vresource_resource_pkg__03a__03aresource__Vclpkg(Vresource__Syms* symsp, const char* v__name);
    ~Vresource_resource_pkg__03a__03aresource__Vclpkg();
    VL_UNCOPYABLE(Vresource_resource_pkg__03a__03aresource__Vclpkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


class Vresource__Syms;

class Vresource_resource_pkg__03a__03aresource : public VlClass {
  public:

    // DESIGN SPECIFIC STATE
    CData/*7:0*/ __PVT__A_comp_id;
    CData/*7:0*/ __PVT__A_pcomp_id;
    CData/*7:0*/ __PVT__A_r_pool_id;
    CData/*7:0*/ __PVT__A_r_inst_id;
    CData/*7:0*/ __PVT__B_comp_id;
    CData/*7:0*/ __PVT__B_pcomp_id;
    CData/*7:0*/ __PVT__B_r_pool_id;
    CData/*7:0*/ __PVT__B_r_inst_id;
    CData/*7:0*/ __PVT__C_comp_id;
    CData/*7:0*/ __PVT__C_pcomp_id;
    CData/*7:0*/ __PVT__C_r_pool_id;
    CData/*7:0*/ __PVT__C_r_inst_id;
    VlRandomizer __PVT__constraint;

    // INTERNAL VARIABLES
    VlRNG __Vm_rng;
    void __VnoInFunc_A_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp);
    void __VnoInFunc_B_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp);
    void __VnoInFunc_C_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp);
    void __VnoInFunc___Vbasic_randomize(Vresource__Syms* __restrict vlSymsp, IData/*31:0*/ &__Vbasic_randomize__Vfuncrtn);
    virtual void __VnoInFunc___Vsetup_constraints(Vresource__Syms* __restrict vlSymsp);
    void __VnoInFunc_print(Vresource__Syms* __restrict vlSymsp);
    void __VnoInFunc_randomize(Vresource__Syms* __restrict vlSymsp, IData/*31:0*/ &randomize__Vfuncrtn);
    void __VnoInFunc_rsrc_c_setup_constraint(Vresource__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset(Vresource__Syms* __restrict vlSymsp);
  public:
    Vresource_resource_pkg__03a__03aresource(Vresource__Syms* __restrict vlSymsp);
    std::string to_string() const;
    std::string to_string_middle() const;
    ~Vresource_resource_pkg__03a__03aresource();
};

std::string VL_TO_STRING(const VlClassRef<Vresource_resource_pkg__03a__03aresource>& obj);

#endif  // guard
