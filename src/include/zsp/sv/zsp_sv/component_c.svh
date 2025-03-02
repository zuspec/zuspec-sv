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
typedef class pool_c;

class component_c extends typed_obj_c;
    int                     comp_id;
    string                  name;
    component_c             parent;

    executor_group_base_c   exec_groups[$];
    executor_base_c         executors[$];
    pool_c                  pool_m;
    executor_group_base_c   exec_group_m[obj_type_c];

    /**
     * Map of component type to list of component instances
     * at this level and below.
     */
    component_queue_h       comp_t_inst_m[obj_type_c];

    component_c             subcomponents[$];


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
        obj_type_c comp_t = comp.get_obj_type();
        `ZSP_DEBUG_ENTER("component_c", ("add_comp_inst %0s (%0s)", comp.name, this.name));
        if (comp_t_inst_m.exists(comp_t)) begin
            comp_t_inst_m[comp_t].push_back(comp);
        end else begin
            component_queue_h l;
            l.push_back(comp);
            comp_t_inst_m[comp_t] = l;
        end
    endfunction

    virtual function void do_init(executor_base_c exec_b);
        `ZSP_DEBUG_ENTER("component_c", ("do_init %0s", this.name));
        init_down(exec_b);

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

endclass
