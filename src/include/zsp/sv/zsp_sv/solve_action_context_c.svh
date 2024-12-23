
typedef class resource_claim_solve_data_c;
typedef component_c comp_s[$];

class solve_action_context_c;
    component_c                         parent_comp;
    resource_claim_solve_data_c         resource_data_m[obj_type_c];
    action_c                            actions[$];
    comp_s                              action_comp_s[$];
`ifndef VERILATOR
    rand resource_claim_solve_data_c    resource_data[$];
    rand int                            comp_idx[];
`else
    resource_claim_solve_data_c         resource_data[$];
    int                                 comp_idx[];
`endif

    function new(component_c parent_comp);
        this.parent_comp = parent_comp;
    endfunction

    function void pre_randomize();
        comp_idx = new[actions.size()];
    endfunction

    function void post_randomize();
        $display("post_randomize");
`ifdef VERILATOR
        foreach (comp_idx[i]) begin
            comp_idx[i] = $urandom_range(0, action_comp_s[i].size()-1);
        end
`endif
        foreach (comp_idx[i]) begin
            $display("Select comp %0d", comp_idx[i]);
            actions[i].set_component(action_comp_s[i][comp_idx[i]]);
        end
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

    function void add_action(action_c action);
        obj_type_c comp_t = action.get_obj_comp_type();
        $display("add_action: comp_t=%0s", comp_t.name);
        $display("parent_comp: %0s", parent_comp.name);
        $display("parent_comp.size: %0d", parent_comp.comp_t_inst_m.size());
        $display("action.comp_obj_type: %0p", action.get_obj_comp_type());

        if (parent_comp.comp_t_inst_m.exists(comp_t)) begin
            actions.push_back(action);
            action_comp_s.push_back(parent_comp.comp_t_inst_m[comp_t]);
            foreach (parent_comp.comp_t_inst_m[comp_t][i]) begin
                $display("comp[%0d] %0s", i, 
                    parent_comp.comp_t_inst_m[comp_t][i].name);
            end
        end else begin
            $display("FATAL: solve_action_context_c::add_action: No component instances for %0s", comp_t.name);
            $finish;
        end
        foreach (action.rsrc_claims[i]) begin
            add_resource_claim(action.rsrc_claims[i]);
        end
    endfunction

    function bit resolve();
        $display("RESOLVE");
        return this.randomize();
    endfunction

endclass