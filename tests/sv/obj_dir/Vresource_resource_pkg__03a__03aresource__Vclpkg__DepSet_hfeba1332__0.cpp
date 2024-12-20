// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource_resource_pkg__03a__03aresource__Vclpkg.h"

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_print(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_print\n"); );
    // Body
    VL_WRITEF_NX("A: comp_id=%0# pcomp_id=%0# r_pool_id=%0# r_inst_id=%0#\nB: comp_id=%0# pcomp_id=%0# r_pool_id=%0# r_inst_id=%0#\nC: comp_id=%0# pcomp_id=%0# r_pool_id=%0# r_inst_id=%0#\n",0,
                 8,this->__PVT__A_comp_id,8,(IData)(this->__PVT__A_pcomp_id),
                 8,this->__PVT__A_r_pool_id,8,(IData)(this->__PVT__A_r_inst_id),
                 8,this->__PVT__B_comp_id,8,(IData)(this->__PVT__B_pcomp_id),
                 8,this->__PVT__B_r_pool_id,8,(IData)(this->__PVT__B_r_inst_id),
                 8,this->__PVT__C_comp_id,8,(IData)(this->__PVT__C_pcomp_id),
                 8,this->__PVT__C_r_pool_id,8,(IData)(this->__PVT__C_r_inst_id));
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_randomize(Vresource__Syms* __restrict vlSymsp, IData/*31:0*/ &randomize__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_randomize\n"); );
    // Init
    IData/*31:0*/ __Vfunc___Vbasic_randomize__1__Vfuncout;
    __Vfunc___Vbasic_randomize__1__Vfuncout = 0;
    IData/*31:0*/ __Vtemp_1;
    // Body
    this->__PVT__constraint.clear();
    this->__PVT__constraint.clear();
    this->__VnoInFunc___Vsetup_constraints(vlSymsp);
    randomize__Vfuncrtn = this->__PVT__constraint.next(__Vm_rng);
    __Vtemp_1 = (randomize__Vfuncrtn & ([&]() {
                this->__VnoInFunc___Vbasic_randomize(vlSymsp, __Vfunc___Vbasic_randomize__1__Vfuncout);
            }(), __Vfunc___Vbasic_randomize__1__Vfuncout));
    randomize__Vfuncrtn = __Vtemp_1;
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_A_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_A_comp_c_setup_constraint\n"); );
    // Body
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) A_pcomp_id) #x00000000))) (__Vbool (bvand (__Vbv (bvuge ((_ zero_extend 24) A_comp_id) #x00000001)) (__Vbv (bvule ((_ zero_extend 24) A_comp_id) #x00000002))))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) A_comp_id) #x00000001))) (__Vbool (__Vbv (= ((_ zero_extend 24) A_r_pool_id) #x00000000)))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) A_comp_id) #x00000002))) (__Vbool (__Vbv (= ((_ zero_extend 24) A_r_pool_id) #x00000001)))))"});
    this->__PVT__constraint.hard(std::string{"(bvand (__Vbv (bvuge ((_ zero_extend 24) A_r_inst_id) #x00000000)) (__Vbv (bvule ((_ zero_extend 24) A_r_inst_id) #x00000001)))"});
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vsetup_constraints(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vsetup_constraints\n"); );
    // Body
    this->__VnoInFunc_A_comp_c_setup_constraint(vlSymsp);
    this->__VnoInFunc_B_comp_c_setup_constraint(vlSymsp);
    this->__VnoInFunc_C_comp_c_setup_constraint(vlSymsp);
    this->__VnoInFunc_rsrc_c_setup_constraint(vlSymsp);
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_B_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_B_comp_c_setup_constraint\n"); );
    // Body
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) B_pcomp_id) #x00000000))) (__Vbool (bvand (__Vbv (bvuge ((_ zero_extend 24) B_comp_id) #x00000001)) (__Vbv (bvule ((_ zero_extend 24) B_comp_id) #x00000002))))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) B_comp_id) #x00000001))) (__Vbool (__Vbv (= ((_ zero_extend 24) B_r_pool_id) #x00000000)))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) B_comp_id) #x00000002))) (__Vbool (__Vbv (= ((_ zero_extend 24) B_r_pool_id) #x00000001)))))"});
    this->__PVT__constraint.hard(std::string{"(bvand (__Vbv (bvuge ((_ zero_extend 24) B_r_inst_id) #x00000000)) (__Vbv (bvule ((_ zero_extend 24) B_r_inst_id) #x00000001)))"});
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_C_comp_c_setup_constraint(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_C_comp_c_setup_constraint\n"); );
    // Body
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) C_pcomp_id) #x00000000))) (__Vbool (bvand (__Vbv (bvuge ((_ zero_extend 24) C_comp_id) #x00000001)) (__Vbv (bvule ((_ zero_extend 24) C_comp_id) #x00000002))))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) C_comp_id) #x00000001))) (__Vbool (__Vbv (= ((_ zero_extend 24) C_r_pool_id) #x00000000)))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (=> (__Vbool (__Vbv (= ((_ zero_extend 24) C_comp_id) #x00000002))) (__Vbool (__Vbv (= ((_ zero_extend 24) C_r_pool_id) #x00000001)))))"});
    this->__PVT__constraint.hard(std::string{"(bvand (__Vbv (bvuge ((_ zero_extend 24) C_r_inst_id) #x00000000)) (__Vbv (bvule ((_ zero_extend 24) C_r_inst_id) #x00000001)))"});
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc_rsrc_c_setup_constraint(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc_rsrc_c_setup_constraint\n"); );
    // Body
    this->__PVT__constraint.hard(std::string{"(__Vbv (= ((_ zero_extend 24) A_pcomp_id) #x00000000))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (= ((_ zero_extend 24) B_pcomp_id) #x00000000))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (= ((_ zero_extend 24) C_pcomp_id) #x00000000))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (not (__Vbool (bvand (__Vbv (= A_r_inst_id B_r_inst_id)) (__Vbv (= A_r_pool_id B_r_pool_id))))))"});
    this->__PVT__constraint.hard(std::string{"(__Vbv (not (__Vbool (bvand (__Vbv (= A_r_inst_id C_r_inst_id)) (__Vbv (= A_r_pool_id C_r_pool_id))))))"});
}

void Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vbasic_randomize(Vresource__Syms* __restrict vlSymsp, IData/*31:0*/ &__Vbasic_randomize__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::__VnoInFunc___Vbasic_randomize\n"); );
    // Body
    __Vbasic_randomize__Vfuncrtn = 1U;
}

Vresource_resource_pkg__03a__03aresource::~Vresource_resource_pkg__03a__03aresource() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vresource_resource_pkg__03a__03aresource>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vresource_resource_pkg__03a__03aresource::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::to_string\n"); );
    // Body
    return ("'{"s + to_string_middle() + "}");
}

std::string Vresource_resource_pkg__03a__03aresource::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aresource::to_string_middle\n"); );
    // Body
    std::string out;
    out += "A_comp_id:" + VL_TO_STRING(__PVT__A_comp_id);
    out += ", A_pcomp_id:" + VL_TO_STRING(__PVT__A_pcomp_id);
    out += ", A_r_pool_id:" + VL_TO_STRING(__PVT__A_r_pool_id);
    out += ", A_r_inst_id:" + VL_TO_STRING(__PVT__A_r_inst_id);
    out += ", B_comp_id:" + VL_TO_STRING(__PVT__B_comp_id);
    out += ", B_pcomp_id:" + VL_TO_STRING(__PVT__B_pcomp_id);
    out += ", B_r_pool_id:" + VL_TO_STRING(__PVT__B_r_pool_id);
    out += ", B_r_inst_id:" + VL_TO_STRING(__PVT__B_r_inst_id);
    out += ", C_comp_id:" + VL_TO_STRING(__PVT__C_comp_id);
    out += ", C_pcomp_id:" + VL_TO_STRING(__PVT__C_pcomp_id);
    out += ", C_r_pool_id:" + VL_TO_STRING(__PVT__C_r_pool_id);
    out += ", C_r_inst_id:" + VL_TO_STRING(__PVT__C_r_inst_id);
    return out;
}
