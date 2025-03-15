/*
 * solve_action_context_c.svh
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
typedef class activity_traverse_base_c;
typedef class component_list_c;
typedef class component_type_c;
typedef class resource_claim_solve_data_c;
typedef class action_type_c;
typedef class solve_compset_c;
typedef component_c comp_s[$];
typedef class ref_claim_type_c;
typedef class solve_pool_ref_c;
typedef class solve_pool2comp_map_c;
typedef class solve_resource_pool_ref_c;
typedef class solve_traversal_data_c;

class solve_action_context_c;
    component_c                         parent_comp;
    resource_claim_solve_data_c         resource_data_m[obj_type_c];

    // Action types that we are scheduling
    action_type_c                       actions[$];
    solve_compset_c                     action_compset_m[action_type_c];

    // List of valid context for each?
    comp_s                              action_comp_s[$];
    int                                 comp_inst_m[component_c];
    component_c                         comp_inst_l[$];

    `zsp_rand_arr resource_claim_solve_data_c    resource_data[$];
    `zsp_rand_arr solve_traversal_data_c         traversal_l[$];
    `zsp_rand_arr solve_resource_pool_ref_c      lock[$];
    `zsp_rand_arr solve_resource_pool_ref_c      share[$];

    function new(component_c parent_comp);
        this.parent_comp = parent_comp;
    endfunction

    function void pre_randomize();
    endfunction

    function void post_randomize();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("post_randomize"));
        `ZSP_DEBUG_LEAVE("solve_action_context_c", ("post_randomize"));
    endfunction

    function void add_resource_claim(resource_claim_base_c claim);
        obj_type_c rsrc_t = claim.get_type();
        resource_claim_solve_data_c data;

        if (resource_data_m.exists(rsrc_t) != 0) begin
            data = resource_data_m[rsrc_t];
        end else begin
            data = new();
            resource_data_m[rsrc_t] = data;
            resource_data.push_back(data);
        end

        data.add_claim(claim);
    endfunction

    function int add_traversal(activity_traverse_base_c traversal);
        component_type_c comp_t;
        solve_compset_c compset;
        solve_traversal_data_c data;
        action_type_c action = traversal.get_action_type();
        int traversal_id = traversal_l.size();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("add_action: action_t=%0s", action.name));
        `ZSP_DEBUG("solve_action_context_c", ("parent_comp: %0s", parent_comp.name));
        `ZSP_DEBUG("solve_action_context_c", ("parent_comp.size: %0d", parent_comp.comp_t_inst_m.size()));

        comp_t = action.get_comp_t();

        compset = get_compset(action);
        data = new(traversal, compset);

        traversal_l.push_back(data);

        // Now, each ref/claim needs to be addressed
        foreach (action.ref_claim_type_l[i]) begin
            ref_claim_type_c claim_t = action.ref_claim_type_l[i];
            pool_base_c pools[$];
            // Specific to action_t (?)
            solve_pool2comp_map_c pool2comp_map = new();

            // Identify the pools that <Action>::<ref> might be associated with
            // in each valid context
            foreach (compset.comp_id_l[i]) begin
                // Must be able to ask <comp> for pool associated with <action_t>::<ref_id>
                pool_base_c pool = comp_inst_l[compset.comp_id_l[i]].get_pool(action, claim_t.id);
                int comp_id = compset.comp_id_l[i];
                pools.push_back(pool);
                pool2comp_map.pool2comp_m.push_back(comp_id);
            end

            if (claim_t.is_claim) begin
                // Resource
            end else begin
                // Flow object
            end
        end

        if (parent_comp.comp_t_inst_m.exists(comp_t) != 0) begin
            // We have available component contexts
            actions.push_back(action);
//            action_comp_s.push_back(parent_comp.comp_t_inst_m[comp_t]);
            // for(int i=0; i<parent_comp.comp_t_inst_m[comp_t].size(); i++) begin
            //     $display("comp[%0d] %0s", i, 
            //     parent_comp.comp_t_inst_m[comp_t][i].name);
            // end
        end else begin
            $display("FATAL: solve_action_context_c::add_action: No component instances for %0s", comp_t.name);
            $finish;
        end

        return traversal_id;
    endfunction

    function solve_compset_c get_compset(action_type_c action);
        if (action_compset_m.exists(action) == 0) begin
            // Need to build a new compset
            if (parent_comp.comp_t_inst_m.exists(action.get_comp_t()) != 0) begin
                // Add each unique component instance to the map
                component_list_c comp_l = parent_comp.comp_t_inst_m[action.get_comp_t()];
                solve_compset_c compset = new();
                foreach (comp_l.comp_l[i]) begin
                    component_c comp = comp_l.comp_l[i];
                    int comp_id;
                    if (comp_inst_m.exists(comp) == 0) begin
                        comp_id = comp_inst_l.size();
                        comp_inst_m[comp] = comp_id;
                        comp_inst_l.push_back(comp);
                    end else begin
                        comp_id = comp_inst_m[comp];
                    end
                    compset.comp_id_l.push_back(comp_id);
                end

                action_compset_m[action] = compset;
            end else begin
                component_type_c key;
                if (parent_comp.comp_t_inst_m.first(key) != 0) begin
                    do begin
                        `ZSP_DEBUG("solve_action_context_c", ("Component type: %0s", key.name));
                    end while (parent_comp.comp_t_inst_m.next(key) != 0);
                end else begin
                    `ZSP_DEBUG("solve_action_context_c", ("No component instances"));
                end
                `ZSP_FATAL(("get_compset: No component instances for %0s in %0s",
                    action.get_comp_t().name,
                    parent_comp.name));
            end
        end
        return action_compset_m[action];
    endfunction

    function action_c mk_action(int action_id);
        // Find selected component
        // Ask to build action of designated type
        // Hook up selected
        action_c action;

        return action;
    endfunction

    function bit resolve();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("resolve"));
        if (this.randomize() == 0) begin
            return 0;
        end

        //
        foreach(traversal_l[i]) begin
            action_type_c action_t = traversal_l[i].activity.get_action_type();
            // Determine which component this is associated with
            component_c comp;
            action_c action;

            `ZSP_DEBUG("solve_action_context_c", ("Resolve action %0s", action_t.name));

            // Use the component to create an action of <type>
            comp = comp_inst_l[traversal_l[i].comp_id];
            action = comp.mk_action(action_t);

            action.set_component(comp);

            traversal_l[i].activity.set_action(action);
        end

        debug_show_result();

        `ZSP_DEBUG_LEAVE("solve_action_context_c", ("resolve"));
        return 1;
    endfunction

    function void debug_show_result();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("debug_show_result"));
        foreach (traversal_l[i]) begin
            `ZSP_DEBUG("solve_action_context_c", ("traversal[%0d]: %0s", i, traversal_l[i].activity.get_action_type().name));
        end
        `ZSP_DEBUG_LEAVE("solve_action_context_c", ("debug_show_result"));
    endfunction


`ifndef VERILATOR
    constraint legal_c {
        // Pool id is an index into the collected legal pools
        foreach (lock[i]) {
`ifdef UNDEFINED
            lock[i].pool_id inside {[0:pools.size-1]};

            foreach (pools[j]) {
                if (lock[i].pool_id == j) {
                    lock[i].id inside {[0:pools[j].resources.size-1]};
                }
            }
`endif // UNDEFINED
        }

        foreach (share[i]) {
`ifdef UNDEFINED
            share[i].pool_id inside {[0:pools.size-1]};

            foreach (pools[j]) {
                if (share[i].pool_id == j) {
                    share[i].id inside {[0:pools[j].resources.size-1]};
                }
            }
`endif // UNDEFINED
        }

`ifdef UNDEFINED
        foreach (lock[i]) {
            lock[i].id inside {[0:pool_max_sz-1]};
        }

        foreach (share[i]) {
            share[i].id inside {[0:pool_max_sz-1]};
        }
`endif // UNDEFINED
    }

    constraint lock_c {
        // Locks must be distinct
        foreach (lock[i]) {
            foreach (lock[j]) {
                if (j > i) {
                    ((lock[i].id != lock[j].id) || (lock[i].pool_id != lock[j].pool_id));
                }
            }
        }

        // Share and lock must be distinct
        foreach (lock[i]) {
            foreach (share[j]) {
                ((lock[i].id != share[j].id) || (lock[i].pool_id != share[j].pool_id));
            }
        }

        // Now, how
    }
`endif // VERILATOR

endclass
