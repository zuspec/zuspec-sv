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
    static function obj_type_c get_comp_type(); \
        return comp_t::get_type(); \
    endfunction \
    virtual function obj_type_c get_obj_comp_type(); \
        return get_comp_type(); \
    endfunction

`define ZSP_DEBUG_ENTER(region, msg) \
    if (zsp_sv::log_level) begin \
        $display("--> %0s::%0s", region, $sformatf msg); \
    end

`define ZSP_DEBUG(region, msg) \
    if (zsp_sv::log_level) begin \
        $display("%0s::%0s", region, $sformatf msg); \
    end

`define ZSP_DEBUG_LEAVE(region, msg) \
    if (zsp_sv::log_level) begin \
        $display("<-- %0s::%0s", region, $sformatf msg); \
    end

`define zsp_print(exec_b, msg) \
    $write msg 

`define zsp_message(exec_b, verb, msg) \
    begin \
        message_verbosity_e v = message_verbosity_e'(verb); \
        if (exec_b.get_actor().verbosity >= v) begin \
            exec_b.get_api().message($sformatf msg ); \
        end \
    end 


`endif /* INCLUDED_ZSP_SV_MACROS_SVH */
