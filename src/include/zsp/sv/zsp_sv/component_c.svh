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
typedef class empty_executor_trait_s;
typedef class executor_group_c;
typedef class executor_group_base_c;
typedef class executor_group_dummy_c;
typedef class pool_base_c;
typedef class pool_map_c;

class component_list_c;
    component_c comp_l[$];
endclass

class component_c extends typed_obj_c;
    `zsp_component_util(component_c)
    int                     comp_id;
    string                  name;
    component_c             parent;
    
    executor_group_base_c   exec_groups[$];
    executor_base_c         executors[$];
    pool_map_c              pool_m;
    executor_group_base_c   exec_group_m[obj_type_c];

    /**
     * Map of component type to list of component instances
     * at this level and below.
     */
    component_list_c        comp_t_inst_m[component_type_c];

    component_c             subcomponents[$];
    action_type_c           action_virtual_m[action_type_c];


    // Each component maintains a map of claim type to executor
    executor_base_c  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        `ZSP_DEBUG_ENTER("component_c", ("new %0s", name));
        //$display("component_c::new: %0s ctxt=%p", name, ctxt);
        this.name = name;
        this.parent = parent;
        if (parent != null) begin
            parent.subcomponents.push_back(this);
        end

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
        component_type_c comp_t;
        `ZSP_DEBUG_ENTER("component_c", ("add_comp_inst %0s", comp.name));
        $cast(comp_t, comp.get_obj_type());
        `ZSP_DEBUG("component_c", ("comp_t: %0s", comp_t.name));
        if (comp_t_inst_m.exists(comp_t)) begin
            comp_t_inst_m[comp_t].comp_l.push_back(comp);
        end else begin
            component_list_c l = new();
            l.comp_l.push_back(comp);
            comp_t_inst_m[comp_t] = l;
        end
        `ZSP_DEBUG_LEAVE("component_c", ("add_comp_inst %0s", comp.name));
    endfunction

    // Implemented by derived types
    virtual function void bind_pools();
        // NOP by default
    endfunction

    virtual function void bind_pool_wildcard(pool_base_c pool);
        `ZSP_DEBUG_ENTER("component_c", ("bind_pool_wildcard %0s: %0s", name, pool.name));
        if (pool_m == null) begin
            pool_m = new();
        end

        pool_m.bind_wildcard(pool);
        `ZSP_DEBUG_LEAVE("component_c", ("bind_pool_wildcard %0s: %0s", name, pool.name));
    endfunction

    function pool_base_c get_pool(action_type_c action_t, int ref_id);
        // See if there is a mapping for action::ref_id
        // Otherwise, find the mapping for action_t.ref_claim_type_l[ref_id].obj_t
        component_c comp = this;
        pool_base_c pool;
        `ZSP_DEBUG_ENTER("component_c", ("get_pool %0s: %0s ref_id=%0d", name, action_t.name, ref_id));

        while (comp != null && pool == null) begin
            if (comp.pool_m != null) begin
                pool = comp.pool_m.get_pool(action_t, ref_id);
                `ZSP_DEBUG_ENTER("component_c", ("check pool-map in %0s", comp.name));
            end else begin
                `ZSP_DEBUG_ENTER("component_c", ("no pool-map in %0s", comp.name));
            end
            comp = comp.parent;
        end

        if (pool == null) begin
            `ZSP_FATAL(("Failed to find pool for action %0s ref_id %0d", action_t.name, ref_id));
        end

        `ZSP_DEBUG_LEAVE("component_c", ("get_pool %0s ref_id=%0d", action_t.name, ref_id));
        return pool;
    endfunction

    virtual function void do_init(executor_base_c exec_b);
        `ZSP_DEBUG_ENTER("component_c", ("do_init %0s", this.name));
        init_down(exec_b);

        bind_pools();

        // Process added components
        foreach (subcomponents[i]) begin
            add_comp_inst(subcomponents[i]);
        end
        add_comp_inst(this);

        // If we're not changnig anything at this
        // level, just take our parent's map
        if (parent != null && exec_groups.size() == 0 && executors.size() == 0) begin
            `ZSP_DEBUG("component_c", ("Taking parent's exec_group_m"));
            exec_group_m = parent.exec_group_m;
        end else begin
            // We're making changes - question is what
            executor_group_base_c exec_group_m[obj_type_c];

            `ZSP_DEBUG("component_c", ("Updating parent's exec_group_m"));

            if (parent != null) begin
                exec_group_m = parent.exec_group_m;
            end

            // Slot in local executor groups
            foreach (exec_groups[i]) begin
                `ZSP_DEBUG("component_c", ("Setting group %0s for type %0s",
                    exec_groups[i].name, exec_groups[i].get_trait_type().name));
                exec_group_m[exec_groups[i].get_trait_type()] = exec_groups[i];
            end

            // Now, see if we need to create a synthetic group
            // for any standalone executors
            foreach (executors[i]) begin
                if (!exec_group_m.exists(executors[i].get_trait_type())) begin
                    executor_group_dummy_c exec_group = new(
                        {"__group_", executors[i].name},
                        null,
                        this,
                        executors[i].get_trait_type());
                    exec_group_m[executors[i].get_trait_type()] = exec_group;
                    exec_group.add_executor(executors[i]);
                    `ZSP_DEBUG("component_c", ("Creating dummy group %0s for type %0s",
                        exec_group.name, executors[i].get_trait_type().name));
                end
            end

            // Save the updated map 
            this.exec_group_m = exec_group_m;
        end

        foreach (subcomponents[i]) begin
            subcomponents[i].do_init(exec_b);
        end

        init_up(exec_b);
        `ZSP_DEBUG_LEAVE("component_c", ("<-- do_init %0s", this.name));
    endfunction

    virtual function void init_down(executor_base_c exec_b);
    endfunction

    virtual function void init(executor_base_c exec_b);
    endfunction

    virtual function void init_up(executor_base_c exec_b);
    endfunction

    virtual function void start(executor_base_c exec_b);
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

    virtual function executor_base_c get_default_executor();
        component_c c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor.get_default_executor();
    endfunction

    virtual function executor_group_base_c get_executor_group(obj_type_c trait_t=null);
        executor_group_base_c ret;
        if (trait_t == null) begin
            trait_t = empty_executor_trait_s::get_type();
        end
        if (exec_group_m.exists(trait_t)) begin
            ret = exec_group_m[trait_t];
        end else begin
            `ZSP_FATAL(("Failed to find executor group for trait %0s", trait_t.name));
        end
        return ret;
    endfunction

    function action_c mk_action(action_type_c action_t);
        action_c action;
        component_type_c comp_t;
        `ZSP_DEBUG_ENTER("component_c", ("mk_action %0s: %0s", name, action_t.name));
        $cast(comp_t, get_obj_type());
        action = comp_t.mk_action(action_t);
        `ZSP_DEBUG_LEAVE("component_c", ("mk_action %0s: %0s", name, action_t.name));
        return action;
    endfunction

endclass
