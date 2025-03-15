/*
 * executor_base_c.svh
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
 

class executor_base_c extends component_c;
    actor_base_c       actor;
    backend_api        api;

    function new(string name, component_ctor_ctxt_c ctxt, component_c parent);
        super.new(name, ctxt, parent);

        if (parent != null) begin
            parent.executors.push_back(this);
        end
    endfunction

    virtual function obj_type_c get_trait_type();
        `ZSP_FATAL(("get_trait_type not implemented"));
        return null;
    endfunction

    function backend_api get_api();
        if (api == null) begin
            component_c c = this;
            actor_base_c actor;
            while (c.parent != null) begin
                c = c.parent;
            end
            if ($cast(actor, c) == 0) begin
                `ZSP_FATAL(("failed to cast root component_c to actor_base_c"));
            end
            api = actor.get_backend();
        end
        if (api == null) begin
            `ZSP_FATAL(("failed to get backend API"));
        end
        return api;
    endfunction

    function actor_base_c get_actor();
        if (actor == null) begin
            component_c c = this;
            while (c.parent != null) begin
                c = c.parent;
            end
            if (!$cast(actor, c)) begin
                $display("Error: failed to cast root component_c to actor_base_c");
            end
        end
        return actor;
    endfunction

    virtual function bit[63:0] addr_value(addr_handle_t hndl);
        return hndl.addr_value();
    endfunction

    virtual task read8 (
        input executor_base_c   exec_b,
        output byte unsigned    __retval,
        input addr_handle_t     hndl);
        `ZSP_FATAL(("read8 not implemented"));
    endtask

    virtual task read16(
        input executor_base_c   exec_b,
        output shortint unsigned __retval,
        input addr_handle_t     hndl);
        `ZSP_FATAL(("read16 not implemented"));
    endtask

    virtual task read32(
        input executor_base_c   exec_b,
        output int unsigned     __retval,
        input addr_handle_t     hndl);
        `ZSP_FATAL(("read32 not implemented"));
    endtask

    virtual task read64(
        input executor_base_c   exec_b,
        output longint unsigned __retval,
        input addr_handle_t     hndl);
        `ZSP_FATAL(("read64 not implemented"));
    endtask

    virtual task write8 (
        executor_base_c         exec_b,
        addr_handle_t           hndl, 
        byte unsigned           data);
        `ZSP_FATAL(("write8 not implemented"));
    endtask

    virtual task write16(
        executor_base_c         exec_b,
        addr_handle_t           hndl,
        shortint unsigned       data);
        `ZSP_FATAL(("write16 not implemented"));
    endtask

    virtual task write32(
        executor_base_c         exec_b,
        addr_handle_t           hndl,
        int unsigned            data);
        `ZSP_FATAL(("write32 not implemented"));
    endtask

    virtual task write64(
        executor_base_c         exec_b,
        addr_handle_t           hndl,
        longint unsigned        data);
        `ZSP_FATAL(("write64 not implemented"));
    endtask

    virtual task read_bytes (addr_handle_t hndl, bit[7:0] data[$], int size);
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

    virtual task write_bytes(addr_handle_t hndl, bit[7:0] data[$]);
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

endclass
