/*
 * zsp_sv.sv
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

`include "zsp_sv_macros.svh"

package zsp_sv;

`ifdef UNDEFINED
typedef class actor_base_c;
typedef class component_c;
typedef class executor_base_c;
typedef class executor_base_c;
`endif
    `include "array_c.svh"
    `include "list_c.svh"

    typedef enum {NONE, LOW, MEDIUM, HIGH, FULL} message_verbosity_e;

    int log_level = FULL;

class empty_t;
endclass

    `include "object_c.svh"
    `include "object_mgr_c.svh"
    `include "object_refcnt_c.svh"
    `include "object_ref_base_c.svh"
    `include "object_ref_c.svh"

    `include "obj_type_c.svh"
    `include "typed_obj_c.svh"

    `include "flow_obj_c.svh"
    `include "pool_base_c.svh"
    `include "pool_map_c.svh"
    `include "pool_resource_c.svh"
    `include "resource_c.svh"
    `include "resource_claim_base_c.svh"
    `include "resource_claim_c.svh"

    `include "solve_action_context_c.svh"
    `include "solve_compset_c.svh"
    `include "solve_pool_ref_c.svh"
    `include "solve_pool2comp_c.svh"
    `include "solve_pool2comp_map_c.svh"
    `include "solve_resource_claim_data_c.svh"
    `include "solve_resource_pool_ref_c.svh"
    `include "solve_traversal_data_c.svh"

interface class packed_s;

    pure virtual function bit[1023:0] pack();

    pure virtual function void pack_bytes(byte unsigned data[$]);

endclass

    `include "storage_handle_c.svh"
    `include "addr_handle_t.svh"

    `include "empty_addr_trait_s.svh"
    `include "addr_region_base_s.svh"
    `include "addr_region_s.svh"
    `include "transparent_addr_region_s.svh"

    `include "input_c.svh"
    `include "output_c.svh"

    typedef class addr_claim_base_s;

function automatic addr_handle_t make_handle_from_claim(
        executor_base_c   exec_b,
        addr_claim_base_s claim,
        bit[63:0]         offset);
    addr_handle_t ret;
    ret = new(claim.get(), offset);
    return ret;
endfunction

function automatic addr_handle_t make_handle_from_handle(
    executor_base_c     exec_b,
    addr_handle_t       hndl,
    bit[63:0]           offset);
    addr_handle_t ret;
    storage_handle_c base;

    if (hndl != null) begin
        if (hndl.obj.obj != null) begin
            base = hndl.obj.get();
        end
        offset += hndl.offset;
    end

    ret = new(base, offset);

    return ret;
endfunction


    `include "activity_c.svh"
    `include "activity_ctxt_c.svh"
    `include "activity_ctxt_par_c.svh"
    `include "activity_ctxt_par_branch_c.svh"
    `include "activity_ctxt_root_c.svh"
    `include "activity_ctxt_seq_c.svh"
    `include "activity_proxy_c.svh"
    `include "activity_traverse_c.svh"
    `include "activity_traverse_base_c.svh"
`ifdef UNDEFINED
    `include "activity_traverse_compound_c.svh"
`endif // UNDEFINED 
    `include "action_c.svh"
    `include "action_constraint_c.svh"
    `include "action_constraint_base_c.svh"
    `include "action_handle_base_c.svh"
    `include "action_handle_c.svh"
    `include "action_init_c.svh"
    `include "action_type_c.svh"
    `include "component_type_c.svh"

    `include "activity_listener_c.svh"
`ifdef UNDEFINED
`endif // UNDEFINED

`include "component_ctor_ctxt_c.svh"
//`include "component_init_ctxt_c.svh"
    `include "component_c.svh"
    `include "actor_base_c.svh"
    `include "actor_c.svh"
    `include "actor_t_c.svh"

`include "addr_claim_base_s.svh"
`include "addr_trait_s.svh"
`include "addr_space_base_c.svh"
`include "transparent_addr_space_c.svh"

class addr_space_c extends component_c;
    function new(string name, component_ctor_ctxt ctxt, component_c parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function addr_handle_t add_nonallocatable_region(addr_region_base_s region);
        addr_handle_t ret = new(null, region.addr);
        return ret;
    endfunction
endclass

    typedef class reg_group_c;
    `include "reg_field_c.svh"
    `include "reg_group_c.svh"
    `include "reg_group_field_base_c.svh"
    `include "reg_group_field_arr_c.svh"
    `include "reg_group_field_c.svh"
`ifdef UNDEFINED
    `include "hndl_drop_listener.svh"
`endif // UNDEFINED

    `include "backend_api.svh"
    `include "executor_trait_s.svh"
    `include "empty_executor_trait_s.svh"

    `include "executor_base_c.svh"
    `include "executor_c.svh"

    `include "executor_group_base_c.svh"
//    `include "executor_group_default_c.svh"
    `include "executor_group_dummy_c.svh"
    `include "executor_group_c.svh"


    function void activity_bind(
        action_handle_base_c    h1,
        int                     h1_offset,
        action_handle_base_c    h2,
        int                     h2_offset);
        h1.add_activity_bind(
            h1_offset,
            h2.bind_proxies[h2_offset]);
        h2.add_activity_bind(
            h2_offset,
            h1.bind_proxies[h1_offset]);
    endfunction

endpackage


