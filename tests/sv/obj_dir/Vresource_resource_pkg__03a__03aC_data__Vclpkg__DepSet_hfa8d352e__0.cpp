// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vresource.h for the primary calling header

#include "Vresource__pch.h"
#include "Vresource_resource_pkg__03a__03aC_data__Vclpkg.h"

void Vresource_resource_pkg__03a__03aC_data::__VnoInFunc_print(Vresource__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aC_data::__VnoInFunc_print\n"); );
    // Body
    VL_WRITEF_NX("C: comp_id=%0# pcomp_id=%0# r_pool_id=%0# r_inst_id=%0#\n",0,
                 8,this->__PVT__comp_id,8,(IData)(this->__PVT__pcomp_id),
                 8,this->__PVT__r_pool_id,8,(IData)(this->__PVT__r_inst_id));
}

Vresource_resource_pkg__03a__03aC_data::~Vresource_resource_pkg__03a__03aC_data() {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aC_data::~\n"); );
}

std::string VL_TO_STRING(const VlClassRef<Vresource_resource_pkg__03a__03aC_data>& obj) {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aC_data::VL_TO_STRING\n"); );
    // Body
    return (obj ? obj->to_string() : "null");
}

std::string Vresource_resource_pkg__03a__03aC_data::to_string() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aC_data::to_string\n"); );
    // Body
    return ("'{"s + to_string_middle() + "}");
}

std::string Vresource_resource_pkg__03a__03aC_data::to_string_middle() const {
    VL_DEBUG_IF(VL_DBG_MSGF("+          Vresource_resource_pkg__03a__03aC_data::to_string_middle\n"); );
    // Body
    std::string out;
    out += "comp_id:" + VL_TO_STRING(__PVT__comp_id);
    out += ", pcomp_id:" + VL_TO_STRING(__PVT__pcomp_id);
    out += ", r_pool_id:" + VL_TO_STRING(__PVT__r_pool_id);
    out += ", r_inst_id:" + VL_TO_STRING(__PVT__r_inst_id);
    return out;
}
