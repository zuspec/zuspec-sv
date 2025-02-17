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
class component_queue_h;
    component_c comps[$];
endclass


typedef pool_base_c action_type_bind_entry[int];

class type_pool_bind_entry_c;
    action_type_bind_entry  action_t_m[action_type_c];

    function pool_base_c get_pool(action_type_c action_t, int refclaim_id);
        if (action_t_m.exists(action_t)) begin
            return action_t_m[action_t][refclaim_id];
        end else begin
            `ZSP_FATAL(("action_t %0s / ref %0d is not bound", action_t.name, refclaim_id));
        end
        return null;
    endfunction
endclass


class component_c extends typed_obj_c;
    int                     comp_id;
    string                  name;
    component_c             parent;
    component_c             children[$];
    pool_base_c             pool_l[$];
    action_type_bind_entry  refclaim_pool_m[action_type_c];


    /**
     * Map of component type to list of component instances
     * at this level and below.
     */
    component_queue_h       comp_t_inst_m[component_type_c];


    // Each component needs a map of action-claim refs to pool
    executor_base  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        `ZSP_DEBUG_ENTER("component_c", ("new %0s", name));
        //$display("component_c::new: %0s ctxt=%p", name, ctxt);
        this.name = name;
        this.parent = parent;

        if (parent != null) begin
            parent.children.push_back(this);
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
    // function void register_comp_inst(component_c comp);
    //     component_type_c comp_t;

    //     $cast(comp_t, comp.get_obj_type());

    //     if (comp_t_inst_m.exists(comp_t)) begin
    //         comp_t_inst_m[comp_t].comps.push_back(comp);
    //     end else begin
    //         component_queue_h l = new();
    //         l.comps.push_back(comp);
    //         comp_t_inst_m[comp_t] = l;
    //     end
    // endfunction

    function void add_pool(pool_base_c pool);
//        pool.id = pool_l.size();
        pool_l.push_back(pool);
    endfunction

    virtual function void do_init(
        executor_base           exec_b,
        component_init_ctxt_c   ctxt=null);
        `ZSP_DEBUG_ENTER("component_c", ("do_init %0s", name));

        if (ctxt == null) begin
            ctxt = new();
        end

        // if (parent != null) begin
        //     parent.register_comp_inst(this);
        // end

        // First, call the local init_down
        init_down(exec_b);

        // Process children
        foreach (children[i]) begin
            children[i].do_init(exec_b, ctxt);
        end

        // Finally, call the local init_up
        init_up(exec_b);

        // Create a component-instance map for this level
        foreach (children[i]) begin
            component_type_c child_comp_t;
            `ZSP_DEBUG("component_c", ("children[i]: %0s", children[i].name));
            if (children[i].comp_t_inst_m.first(child_comp_t)) begin
                `ZSP_DEBUG("component_c", ("child_comp_t=%0s", child_comp_t.name));
                do begin
                    component_queue_h this_comp_l;
                    component_queue_h child_comps = children[i].comp_t_inst_m[child_comp_t];
                    if (!comp_t_inst_m.exists(child_comp_t)) begin
                        this_comp_l = new();
                        `ZSP_DEBUG("component_c", ("%0s does not have an entry", name));
                        comp_t_inst_m[child_comp_t] = this_comp_l;
                    end else begin
                        `ZSP_DEBUG("component_c", ("%0s already has an entry", name));
                        this_comp_l = comp_t_inst_m[child_comp_t];
                    end

                    foreach (child_comps.comps[j]) begin
                        `ZSP_DEBUG("component_c", ("child_comps.comps[%0d]: %0s", j, child_comps.comps[j].name));
                        this_comp_l.comps.push_back(child_comps.comps[j]);
                    end
                end while (children[i].comp_t_inst_m.next(child_comp_t));
            end
        end
        // Add an entry for ourselves
        begin
            component_queue_h this_comp_l = new();
            component_type_c comp_t;
            $cast(comp_t, get_obj_type());
            this_comp_l.comps.push_back(this);
            comp_t_inst_m[comp_t] = this_comp_l;
        end


        // At this point, all construction and initialization
        // have been performed. We need to build the bind
        // map for this component
        do_bind(ctxt);

        `ZSP_DEBUG_LEAVE("component_c", ("do_init %0s", name));
    endfunction

    virtual function void do_bind(component_init_ctxt_c ctxt);
        component_type_c comp_t;
        pool_base_c pool;
        `ZSP_DEBUG_ENTER("component_c", ("do_bind %0s", name));

        ctxt.push_scope(this);

        // Apply bind information from this scope
        bind_pools(ctxt);

        // Process all action types, and all claim/ref info
        $cast(comp_t, get_obj_type());
        foreach (comp_t.actions[i]) begin
            type_pool_bind_entry_c type_bind_entry = new();
            $display("action %0s", comp_t.actions[i].name);

            foreach (comp_t.actions[i].ref_claim_type_l[j]) begin
                obj_type_c item_c = comp_t.actions[i].ref_claim_type_l[j].obj_t;

                if (!refclaim_pool_m.exists(comp_t.actions[i])) begin
                    action_type_bind_entry entry;
                    refclaim_pool_m[comp_t.actions[i]] = entry;
                end

                // Need to find the pool to use for this action type in this scope
                $display("  claim %0s", comp_t.actions[i].ref_claim_type_l[j].name);
                pool = ctxt.get_pool(comp_t.actions[i], j);
                if (pool == null) begin
                    `ZSP_FATAL(("No pool found for %0s", comp_t.actions[i].ref_claim_type_l[j].name));
                end

                refclaim_pool_m[comp_t.actions[i]][j] = pool;
            end
        end

        // We need to have a map:
        // flow_obj_t -> map_info
        // map_info:
        // - default pool (if any)
        // - action_t -> ref_id -> pool


        foreach (children[i]) begin
            children[i].do_bind(ctxt);
        end

        ctxt.pop_scope();

        `ZSP_DEBUG_LEAVE("component_c", ("do_bind %0s", name));
    endfunction


    virtual function void bind_pools(component_init_ctxt_c ctxt);
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
