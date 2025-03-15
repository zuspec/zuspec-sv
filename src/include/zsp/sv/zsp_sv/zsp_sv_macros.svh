/*
 * zsp_sv_macros.svh
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
`ifndef INCLUDED_ZSP_SV_MACROS_SVH
`define INCLUDED_ZSP_SV_MACROS_SVH

`ifndef ZSP_NO_STACKTRACE
`ifndef ZSP_HAS_STACKTRACE

`ifndef VERILATOR
`define ZSP_HAS_STACKTRACE
`endif

`endif
`endif


`define zsp_reg_type_offset(regtype, regname) regtype``::``prv_inst.``regname``.``offset

`define zsp_reg_type_arr_offset(regtype, regname, index) regtype``::``prv_inst.``regname``.``offsets[``index``]

`define zsp_inc(obj) if (obj != null) obj .inc()

`define zsp_dec(obj) if (obj != null) obj .dec()

`define zsp_typed_obj_util(tname) \
    static obj_type_c __type; \
    static function obj_type_c get_type(); \
        if (__type == null) begin \
            __type = new(`"tname`"); \
        end \
        return __type; \
    endfunction \
    virtual function obj_type_c get_obj_type(); \
        return get_type(); \
    endfunction

`define zsp_action_comp_type(comp_t) \
    static function component_type_c get_comp_type(); \
        return comp_t::get_type(); \
    endfunction \
    virtual function obj_type_c get_obj_comp_type(); \
        return get_comp_type(); \
    endfunction

`define zsp_action_ref_claim_util(obj_t) \
    __type.ref_claim_type_l.push_back(obj_t);

`define zsp_action_util_begin(action_t, comp_t) \
    comp_t comp; \
    \
    virtual function action_c mk(); \
        action_t act = new(); \
        return act; \
    endfunction \
    \
    virtual function void set_component(component_c comp); \
        if (!$cast(this.comp, comp)) begin \
            `ZSP_FATAL(("Failed to assign comp")); \
        end \
    endfunction \
    \
    virtual function component_c get_component(); \
        return comp; \
    endfunction \
    \
    static action_type_t_c #( action_t ) __type = get_type(); \
    static function action_type_t_c #( action_t ) get_type(); \
        if (__type == null) begin \
            __type = new(`"action_t`", comp_t ::get_type());

`define zsp_action_util_ovr_begin(action_t, comp_t) \
    comp_t comp; \
    \
    virtual function action_c mk(); \
        action_t act = new(); \
        return act; \
    endfunction \
    \
    virtual function void set_component(component_c comp); \
        if (!$cast(this.comp, comp)) begin \
            `ZSP_FATAL(("Failed to assign comp")); \
        end \
    endfunction \
    \
    virtual function component_c get_component(); \
        return comp; \
    static action_type_t_c #( action_t ) __type = get_type(); \
    static function action_type_t_c #( action_t ) get_type(); \
        if (__type == null) begin \
            __type = new(`"action_t`", comp_t ::get_type(), 1);

// Register ref/claim objects here
`define zsp_action_util_ref(name,obj_t) \
    __type.add_ref(`"name`", obj_t);

`define zsp_action_util_ref_arr(name,obj_t,count) \
    __type.add_ref(`"name`", obj_t, count);

`define zsp_action_util_lock(name,obj_t) \
    __type.add_lock(`"name`", obj_t ::get_type());

`define zsp_action_util_lock_arr(name,obj_t,count) \
    __type.add_lock(`"name`", obj_t ::get_type(), count);

`define zsp_action_util_share(name, obj_t) \
    __type.add_share(`"name`", obj_t);

`define zsp_action_util_share_arr(name, obj_t,count) \
    __type.add_share(`"name`", obj_t, count);

`define zsp_action_util_end \
        end \
        return __type; \
    endfunction \
    \
    virtual function obj_type_c get_obj_type(); \
        return get_type(); \
    endfunction 


`define zsp_component_util(comp_t) \
    static component_type_c __type; \
    static function component_type_c get_type(); \
        if (__type == null) begin \
            __type = new(`"comp_t`"); \
        end \
        return __type; \
    endfunction \
    virtual function obj_type_c get_obj_type(); \
        return comp_t ::get_type(); \
    endfunction

`define zsp_component_util_inh(comp_t, super_t) \
    static component_type_c __type; \
    static function component_type_c get_type(); \
        if (__type == null) begin \
            __type = new(`"comp_t`", super_t ::get_type()); \
        end \
        return __type; \
    endfunction \
    virtual function obj_type_c get_obj_type(); \
        return comp_t ::get_type(); \
    endfunction


`define ZSP_DEBUG_ENTER(region, msg) \
    if (zsp_sv::log_level != 0) begin \
        $display("--> %0s::%0s", region, $sformatf msg); \
    end

`define ZSP_DEBUG(region, msg) \
    if (zsp_sv::log_level != 0) begin \
        $display("%0s::%0s", region, $sformatf msg); \
    end

`define ZSP_DEBUG_LEAVE(region, msg) \
    if (zsp_sv::log_level != 0) begin \
        $display("<-- %0s::%0s", region, $sformatf msg); \
    end

`define ZSP_FATAL(msg) \
`ifdef ZSP_HAS_STACKTRACE \
    $stacktrace; \
`endif \
    $display("Fatal: %0s", $sformatf msg ); \
    $finish

`define zsp_print(exec_b, msg) \
    exec_b.get_api().print($sformatf msg );

`define zsp_message(exec_b, verb, msg) \
    begin \
        message_verbosity_e v = message_verbosity_e'(verb); \
        if (exec_b.get_actor().verbosity >= v) begin \
            exec_b.get_api().message($sformatf msg ); \
        end \
    end

`ifdef VERILATOR
`define zsp_rand_arr
`else
`define zsp_rand_arr rand
`endif


`endif /* INCLUDED_ZSP_SV_MACROS_SVH */
