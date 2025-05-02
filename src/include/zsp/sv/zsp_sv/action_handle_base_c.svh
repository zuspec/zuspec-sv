
typedef class action_constraint_base_c;
typedef class action_c;
typedef class action_type_c;
typedef class object_ref_base_c;
typedef class ref_claim_type_c;

class action_ref_bind_proxy_c;
    ref_claim_type_c            ref_claim_t;
    action_ref_bind_proxy_c     in_binds[$];
    action_ref_bind_proxy_c     out_binds[$];

    function new(ref_claim_type_c ref_claim_t);
        this.ref_claim_t = ref_claim_t;
    endfunction

endclass

class action_handle_base_c extends object_ref_base_c;
    string                                  name;
    rand action_c                           action;
    action_ref_bind_proxy_c                 bind_proxies[];
    `zsp_rand_arr action_constraint_base_c  traversal_constraints[$];

    function new(string name, action_type_c act_type);
        super.new(null);
        this.name = name;
        this.bind_proxies = new[act_type.ref_claim_type_l.size()];

        foreach (this.bind_proxies[i]) begin
            this.bind_proxies[i] = new(act_type.ref_claim_type_l[i]);
        end
    endfunction

    function void add_activity_bind(
        int                     ref_id,
        action_ref_bind_proxy_c bind_proxy);

        if (!bind_proxy.ref_claim_t.is_claim) begin
            if (bind_proxy.ref_claim_t.is_out) begin
                this.bind_proxies[ref_id].out_binds.push_back(bind_proxy);
            end else begin
                this.bind_proxies[ref_id].in_binds.push_back(bind_proxy);
            end
        end
    endfunction

    virtual function action_c mk_action();
    endfunction

    virtual task run(activity_ctxt_c ctxt);
        executor_base_c exec_b = action.get_executor(ctxt);
        if (this.action == null) begin
            `ZSP_FATAL(("action_handle_c::run: action is null"));
        end

        action.pre_solve(exec_b);
        if (this.randomize() == 0) begin
            `ZSP_FATAL(("Failed to randomize"));
        end
        action.post_solve(exec_b);

        this.action.run(ctxt);
    endtask

    // Apply settings here
    virtual function bit initialize(component_c comp);
//        if (this.action == null) begin
//            `ZSP_FATAL(("action_handle_c::initialize: action is null"));
//        end

        foreach (this.bind_proxies[i]) begin
            if (this.bind_proxies[i].ref_claim_t.is_claim) begin
                // TODO:
//                this.bind_proxies[i].ref_claim_t.initialize(comp);
            end
        end

        // foreach in_ref:
        // - get bound object (must return found,obj)
        // - collect unbound objects
        // Iterate 
        return 0;
    endfunction

endclass
