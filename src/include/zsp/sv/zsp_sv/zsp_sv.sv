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


typedef class object_pool_base;
typedef class actor_c;
typedef class component_c;
typedef class executor_base;

    `include "array_c.svh"
    `include "list_c.svh"

    typedef enum {NONE, LOW, MEDIUM, HIGH, FULL} message_verbosity_e;

    int log_level = 0;

class empty_t;
endclass

class object_pool_base;
    int     count;

    virtual function void inc();
        count += 1;
    endfunction

    virtual function void dec();
        if (count) begin
            count -= 1;
            if (!count) begin
                // Object is no longer referenced
                drop();
            end
        end
    endfunction

    virtual function void drop();
    endfunction

//    virtual function void release(object obj);
//    endfunction
endclass

class object extends object_pool_base;

//    virtual function void init(executor_base exec_b);
//    endfunction

    virtual function void dtor();
    endfunction

    virtual function void do_pre_solve();
    endfunction

    virtual function void pre_solve();
    endfunction

    virtual function void do_post_solve(executor_base exec_b);
    endfunction
    
    virtual function void post_solve(executor_base exec_b);
    endfunction

endclass

    `include "obj_type_c.svh"
    `include "typed_obj_c.svh"

    `include "flow_obj_c.svh"
    `include "pool_c.svh"
    `include "pool_resource_c.svh"
    `include "resource_c.svh"
    `include "resource_claim_c.svh"

    `include "solve_action_context_c.svh"
    `include "solve_resource_claim_data_c.svh"

interface class packed_s;

    pure virtual function bit[1023:0] pack();

    pure virtual function void pack_bytes(byte unsigned data[$]);

endclass



    `include "storage_handle_s.svh"
    `include "addr_handle_t.svh"



class addr_claim_t;
    storage_handle_s    storage;
endclass

function automatic addr_handle_t make_handle_from_claim(
        executor_base   exec_b,
        addr_claim_t    claim, 
        bit[63:0]       offset);
    addr_handle_t ret;
    ret = new(exec_b, claim.storage, offset);    
    return ret;
endfunction

function automatic addr_handle_t make_handle_from_handle(
    executor_base       exec_b,
    addr_handle_t       hndl,
    bit[63:0]           offset);
    addr_handle_t ret;
    if (hndl != null) begin
        if (hndl.base != null) begin
            ret = new(exec_b, hndl.base, hndl.offset+offset); 
        end else begin
            ret = new(exec_b, null, hndl.offset+offset); 
        end
    end else begin
        ret = new(exec_b, null, offset); 
    end
    return ret;
endfunction


    `include "activity_c.svh"
    `include "activity_traverse_c.svh"
    `include "activity_traverse_compound_c.svh"
    `include "action_c.svh"
    `include "action_constraint_c.svh"

    `include "activity_listener_c.svh"



`include "component_ctor_ctxt_c.svh"
`include "component_c.svh"
`include "actor_t_c.svh"

class addr_region_base_s extends object;
    bit[63:0]           size;
    string              tag;
    bit[63:0]           addr;

    virtual function bit[63:0] get_address();
        return addr;
    endfunction
endclass

class addr_space_c extends component_c;
    function new(string name, component_ctor_ctxt ctxt, component_c parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function addr_handle_t add_nonallocatable_region(addr_region_base_s region);
        addr_handle_t ret = new(null, null, region.addr);
        $display("add_nonallocatable_region: 0x%08h %0d", region.addr, ret.count);
        return ret;
    endfunction
endclass

    typedef class reg_group_c;
    `include "reg_field_c.svh"
    `include "reg_group_c.svh"
    `include "reg_group_field_base_c.svh"
    `include "reg_group_field_arr_c.svh"
    `include "reg_group_field_c.svh"

    `include "hndl_drop_listener.svh"

    `include "backend_api.svh"
    `include "executor_base.svh"
    `include "actor_c.svh"

endpackage


