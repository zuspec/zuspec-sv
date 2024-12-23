
typedef class resource_claim_base_c;

class action_c extends object;
    rand bit[15:0]          parent_comp_id;
    rand bit[15:0]          comp_id;
    resource_claim_base_c   rsrc_claims[$];

    virtual task body(executor_base exec_b);
    endtask

    virtual function int add_resource_claim(resource_claim_base_c rsrc_claim);
        int ret = rsrc_claims.size(); // TODO: Must consider other refs as well
        rsrc_claims.push_back(rsrc_claim);
        return ret;
    endfunction

    virtual function executor_base get_executor();
        return null;
    endfunction

    virtual function void set_component(component_c comp);
        $display("FATAL: set_component not implemented");
        $finish;
    endfunction

    virtual function component_c get_component();
        $display("FATAL: get_component not implemented");
        $finish;
        return null;
    endfunction

    virtual function obj_type_c get_obj_comp_type();
        return null;
    endfunction

endclass
