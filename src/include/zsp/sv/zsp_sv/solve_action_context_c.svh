
class solve_action_context_c;
    component_c                         parent_comp;
    resource_claim_solve_data_c         resource_data_m[obj_type_c];
    rand resource_claim_solve_data_c    resource_data[$];

    function new(component_c parent_comp);
        this.parent_comp = parent_comp;
    endfunction

    function void pre_randomize();
    endfunction

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
        foreach (action.rsrc_claims[i]) begin
            add_resource_claim(action.rsrc_claims[i]);
        end
    endfunction

    function bit resolve();
    endfunction

endclass