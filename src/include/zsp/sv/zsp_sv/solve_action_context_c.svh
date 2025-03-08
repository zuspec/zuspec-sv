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
typedef class component_type_c;
typedef class resource_claim_solve_data_c;
typedef class action_type_c;
typedef class solve_traversal_context_c;
typedef component_c comp_s[$];

class pool_ref_c;
    rand bit[31:0]          pool_id;
    bit[31:0]               pool_id_l[$];
endclass

class resource_pool_ref_c extends pool_ref_c;
    rand bit[31:0]          id;
endclass


class solve_action_context_c;
    component_c                         parent_comp;
    resource_claim_solve_data_c         resource_data_m[obj_type_c];

    // Action types that we are scheduling
    action_type_c                       actions[$];

    // List of valid context for each?
    comp_s                              action_comp_s[$];



`ifndef VERILATOR
    rand bit[31:0]                      comp_id_l[$];
    rand resource_claim_solve_data_c    resource_data[$];
    rand int                            comp_idx[];

    rand solve_traversal_context_c      traversal_l[$];
    rand resource_pool_ref_c            lock[$];
    rand resource_pool_ref_c            share[$];
`else
    bit[31:0]                           comp_id_l[$];
    resource_claim_solve_data_c         resource_data[$];
    int                                 comp_idx[];

    solve_traversal_context_c           traversal_l[$];
    resource_pool_ref_c                 lock[$];
    resource_pool_ref_c                 share[$];
`endif

    function new(component_c parent_comp);
        this.parent_comp = parent_comp;
    endfunction

    function void pre_randomize();
//        comp_idx = new[actions.size()];
    endfunction

    function void post_randomize();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("post_randomize"));
`ifdef VERILATOR
        foreach (comp_idx[i]) begin
            comp_idx[i] = $urandom_range(0, action_comp_s[i].size()-1);
        end
`endif
/*
        foreach (comp_idx[i]) begin
            `ZSP_DEBUG("solve_action_context_c", ("Select comp %0d", comp_idx[i]));
            actions[i].set_component(action_comp_s[i][comp_idx[i]]);
        end
 */
        `ZSP_DEBUG_LEAVE("solve_action_context_c", ("post_randomize"));
    endfunction

`ifndef VERILATOR
    // As of 5.030, Verilator does not support iterative constraints
    constraint comp_idx_c {
        foreach (comp_idx[i]) {
            comp_idx[i] inside {[0:action_comp_s[i].size()-1]};
        }
    }
`endif

    function void add_resource_claim(resource_claim_base_c claim);
        obj_type_c rsrc_t = claim.get_type();
        resource_claim_solve_data_c data;

        if (resource_data_m.exists(rsrc_t)) begin
            data = resource_data_m[rsrc_t];
        end else begin
            data = new();
            resource_data_m[rsrc_t] = data;
            resource_data.push_back(data);
        end

        data.add_claim(claim);
    endfunction

    function int add_action(action_type_c action);
        component_type_c comp_t;
        int action_id = traversal_l.size();
        `ZSP_DEBUG_ENTER("solve_action_context_c", ("add_action: action_t=%0s", action.name));
        `ZSP_DEBUG("solve_action_context_c", ("parent_comp: %0s", parent_comp.name));
        `ZSP_DEBUG("solve_action_context_c", ("parent_comp.size: %0d", parent_comp.comp_t_inst_m.size()));
//        $display("action.comp_obj_type: %0p", action.get_obj_comp_type());
        if (!$cast(comp_t, action.get_comp_t())) begin
            `ZSP_FATAL(("add_action: Failed to cast to component_type_c"));
        end else if (comp_t == null) begin
            `ZSP_FATAL(("add_action: comp_t is null"));
        end

        if (parent_comp.comp_t_inst_m.exists(comp_t)) begin
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
        /*
        foreach (action.rsrc_claims[i]) begin
            add_resource_claim(action.rsrc_claims[i]);
        end
         */
        return action_id;
    endfunction

    function action_c mk_action(int action_id);
        // Find selected component
        // Ask to build action of designated type
        // Hook up selected 
        action_c action;

        return action;
    endfunction

    function bit resolve();
        return this.randomize();
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