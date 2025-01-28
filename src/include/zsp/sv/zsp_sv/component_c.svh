/*
 * component_c.svh
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
typedef component_c component_queue_h[$];
typedef class pool_c;

class component_c extends typed_obj_c;
    int                     comp_id;
    string                  name;
    component_c             parent;
    
    pool_c                pool_m;


    /**
     * Map of component type to list of component instances
     * at this level and below.
     */
    component_queue_h       comp_t_inst_m[obj_type_c];


    // Each component needs a map of action-claim refs to pool
    executor_base  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        `ZSP_DEBUG_ENTER("component_c", ("new %0s", name));
        //$display("component_c::new: %0s ctxt=%p", name, ctxt);
        this.name = name;
        this.parent = parent;
        if (ctxt != null) begin
            this.comp_id = ctxt.actor.comp_l.size();
            ctxt.actor.comp_l.push_back(this);
        end else begin
            this.comp_id = -1;
        end
        `ZSP_DEBUG_LEAVE("component_c", ("new %0s", name));
    endfunction

    /**
     * Adds a component instance to the map of comp_t -> [comp_inst] map
     */
    function void add_comp_inst(component_c comp);
        obj_type_c comp_t = comp.get_obj_type();
        if (comp_t_inst_m.exists(comp_t)) begin
            comp_t_inst_m[comp_t].push_back(comp);
        end else begin
            component_queue_h l;
            l.push_back(comp);
            comp_t_inst_m[comp_t] = l;
        end
    endfunction

    virtual function void init_down(executor_base exec_b);
    endfunction

    virtual function void init(executor_base exec_b);
    endfunction

    virtual function void init_up(executor_base exec_b);
    endfunction

    virtual function void start(executor_base exec_b);
    endfunction

    virtual function bit check();
        return 1;
    endfunction

    virtual function actor_c get_actor();
        component_c c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor;
    endfunction

    virtual function executor_base get_default_executor();
        component_c c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor.get_default_executor();
    endfunction

endclass
