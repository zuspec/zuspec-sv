/*
 * actor_base_c.svh
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

typedef class executor_group_default_c;

class actor_base_c extends component_c;
    component_c                             comp_l[$];
    activity_listener_c                     listeners[$];
    hndl_drop_listener #(addr_handle_t)     addr_handle_drop_listeners[$];
    executor_c                              default_exec;
    message_verbosity_e                     verbosity = MEDIUM;
    // TODO: address-space

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        super.new(name, ctxt, null);
        `ZSP_DEBUG_ENTER("actor_base_c", ("--> new %0s", name));

        exec_group_default = new("exec_group_default", ctxt, this);
        default_exec = new("default_exec", ctxt, this);
        // Add a default executor to the group
        exec_group_default.add_executor(default_exec);

        `ZSP_DEBUG_ENTER("actor_base_c", ("<-- new %0s", name));
    endfunction

    virtual function void do_init(executor_base_c exec_b=null);
        `ZSP_DEBUG_ENTER("actor_base_c", ("do_init"));
        if (exec_b == null) begin
            // Use default executor
            exec_b = default_exec;
        end
        super.do_init(exec_b);
        `ZSP_DEBUG_LEAVE("actor_base_c", ("do_init"));
    endfunction

    virtual task run();
        executor_base_c exec_b = get_default_executor();
        foreach (comp_l[i]) begin
            comp_l[i].start(exec_b);
        end
    endtask

    virtual function void start(executor_base_c exec_b);
        // The root actor's behavior is solely controlled from 'run'
    endfunction

    virtual function void add_listener(activity_listener_c listener);
        listeners.push_back(listener);
    endfunction

    virtual function void add_addr_handle_drop_listener(
        hndl_drop_listener #(addr_handle_t) l);
        addr_handle_drop_listeners.push_back(l);
    endfunction

    virtual function void addr_handle_drop(addr_handle_t hndl);
        foreach (addr_handle_drop_listeners[i]) begin
            addr_handle_drop_listeners[i].drop(hndl);
        end
    endfunction

    virtual function backend_api get_backend();
        return null;
    endfunction

    virtual function executor_base_c get_default_executor();
        return default_exec;
    endfunction

endclass
