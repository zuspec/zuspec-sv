
typedef class action_type_c;
typedef class obj_type_c;
typedef class pool_base_c;
typedef class ref_claim_type_c;

class pool_map_c;
    pool_base_c     wildcard_m[obj_type_c];

    function void bind_wildcard(pool_base_c pool);
        `ZSP_DEBUG_ENTER("pool_map_c", ("bind_wildcard %0s <=> %0s", pool.obj_t.name, pool.name));
        wildcard_m[pool.obj_t] = pool;
        `ZSP_DEBUG_LEAVE("pool_map_c", ("bind_wildcard %0s <=> %0s", pool.obj_t.name, pool.name));
    endfunction

    function pool_base_c get_pool(action_type_c action_t, int ref_id);
        pool_base_c pool;
        ref_claim_type_c ref_claim = action_t.ref_claim_type_l[ref_id];

        `ZSP_DEBUG_ENTER("pool_map_c", ("get_pool %0s::%0s (%0s)", action_t.name, ref_claim.name, ref_claim.obj_t.name));

        if (wildcard_m.exists(ref_claim.obj_t) != 0) begin
            pool = wildcard_m[ref_claim.obj_t];
        end

        `ZSP_DEBUG_LEAVE("pool_map_c", ("get_pool %0s::%0s (%0s)", action_t.name, ref_claim.name, ref_claim.obj_t.name));

        return pool;
    endfunction


endclass
