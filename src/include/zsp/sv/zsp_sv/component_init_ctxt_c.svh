
typedef class pool_base_c;

class bind_spec_wildcard_c;
    component_c     scope;
    pool_base_c     pool;
    // There's a path here somewhere...

    function new(component_c scope, pool_base_c pool);
        this.scope = scope;
        this.pool = pool;
    endfunction
endclass

class bind_spec_refclaim_c;
    // There's an action path here somewhere...
    component_c     scope;
    action_type_c   action_t;
    int             refclaim_id;
    pool_base_c     pool;

    function new(
        component_c     scope,
        action_type_c   action_t,
        int             refclaim_id,
        pool_base_c     pool);
        this.scope = scope;
        this.action_t = action_t;
        this.refclaim_id = refclaim_id;
        this.pool = pool;
    endfunction
endclass


// Map of ref ID in the action to pool
typedef pool_base_c ref_pool_m[int];

class obj_bind_info_c;
    pool_base_c     pool;
    ref_pool_m      action_ref_m[action_type_c];
endclass

class pool_bind_context_c;
    bind_spec_wildcard_c    pending_wildcard_binds[$];
    bind_spec_refclaim_c    pending_refclaim_binds[$];

    // Current bind info for each flow-object type
    obj_bind_info_c         active_bind_m[obj_type_c];
endclass

class component_init_ctxt_c;
    bind_spec_wildcard_c    bind_spec_wildcard_l[$];
    pool_bind_context_c     pool_bind_ctxt_s[$];

    virtual function void push_scope(component_c scope);
        // Check the pending binds to see if we should activate any
        pool_bind_context_c bind_ctxt = new();

        if (pool_bind_ctxt_s.size() > 0) begin
            // Clone the previous context before pushing
            pool_bind_context_c ex_bind_ctxt = this.scope();

            // Determine whether any of the pending binds become active here
        end

        pool_bind_ctxt_s.push_back(bind_ctxt);
    endfunction

    virtual function void pop_scope();
        void'(pool_bind_ctxt_s.pop_back());
    endfunction

    function pool_bind_context_c scope();
        return pool_bind_ctxt_s[pool_bind_ctxt_s.size()-1];
    endfunction


    virtual function void bind_pool_wildcard(pool_base_c pool, component_c scope=null);
        `ZSP_DEBUG_ENTER("component_init_ctxt_c", ("bind_pool_wildcard %0s", pool.get_item_type().name));
        if (scope != null) begin
        end

        if (scope == null) begin
            pool_bind_context_c ex_bind_ctxt = this.scope();
            obj_type_c item_t = pool.get_item_type();

            // Add a new entry if one doesn't exist 
            if (ex_bind_ctxt.active_bind_m.exists(item_t) == 0) begin
                obj_bind_info_c bind_info = new();
                bind_info.pool = pool;
                ex_bind_ctxt.active_bind_m[item_t] = bind_info;
            end else begin
                obj_bind_info_c bind_info = ex_bind_ctxt.active_bind_m[item_t];
                if (bind_info.pool != null) begin
                    bind_info.pool = pool;
                end
            end
        end else begin
            // Save the bind directive for later
            `ZSP_FATAL(("bind_pool_wildcard: scope not yet implemented"));
        end

        `ZSP_DEBUG_LEAVE("component_init_ctxt_c", ("bind_pool_wildcard %0s", pool.get_item_type().name));
    endfunction

    virtual function pool_base_c get_pool(action_type_c action_t, int refclaim_id);
        pool_base_c pool;
        obj_type_c item_t = action_t.ref_claim_type_l[refclaim_id].obj_t;
        pool_bind_context_c ex_bind_ctxt = scope();
        `ZSP_DEBUG_ENTER("component_init_ctxt_c", ("get_pool: action_t=%0s refclaim_id=%0d", 
            action_t.name, refclaim_id));

        if (ex_bind_ctxt.active_bind_m.exists(item_t) == 0) begin
            `ZSP_DEBUG("component_init_ctxt_c", ("get_pool: No active bind for object type %0s", item_t.name));
        end else begin
            obj_bind_info_c bind_info = ex_bind_ctxt.active_bind_m[item_t];
            if (bind_info.action_ref_m.exists(action_t) != 0) begin
                if (bind_info.action_ref_m[action_t].exists(refclaim_id) != 0) begin
                end else begin
                    pool = bind_info.pool;
                end
                return bind_info.pool;
            end else begin
                pool = bind_info.pool;
            end
        end

        // Look for a specific bind first
        `ZSP_DEBUG_LEAVE("component_init_ctxt_c", ("get_pool: action_t=%0s refclaim_id=%0d => %0s",
            action_t.name, refclaim_id, pool.name));
        return pool;
    endfunction

endclass
