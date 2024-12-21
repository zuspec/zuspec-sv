
typedef struct {
    int            pool_id;
    int            id;
} resource_pool_ref_t;

/**
 * Solve data for claims against a single resource type involving
 * one or more actions and pools
 */
class resource_claim_solve_data_c;
    // Reachable claim pools, indexed by valid component index
    pool_resource_c             pools[$]; 
    int                         pool_max_sz;
    rand resource_pool_ref_t    lock[$];
    resource_claim_base_c       lock_claim[$];
    rand resource_pool_ref_t    share[$];
    resource_claim_base_c       share_claim[$];

    function void pre_randomize();
        // Bound resource request by largest pool
        foreach (pools[i]) begin
            if (pools[i].resources.size > pool_max_sz) begin
                pool_max_sz = pools[i].resources.size;
            end
        end
    endfunction

    function void add_claim(resource_claim_base_c claim);
        if (claim.lock) begin
            lock.push_back('{0,0});
            lock_claim.push_back(claim);
        end else begin
            share.push_back('{0,0});
            share_claim.push_back(claim);
        end
    endfunction

    constraint legal_c {
        // Pool id is an index into the collected legal pools
        foreach (lock[i]) {
            lock[i].pool_id inside {[0:pools.size-1]};

            foreach (pools[j]) {
                if (lock[i].pool_id == j) {
                    lock[i].id inside {[0:pools[j].resources.size-1]};
                }
            }
        }

        foreach (share[i]) {
            share[i].pool_id inside {[0:pools.size-1]};

            foreach (pools[j]) {
                if (share[i].pool_id == j) {
                    share[i].id inside {[0:pools[j].resources.size-1]};
                }
            }
        }

        foreach (lock[i]) {
            lock[i].id inside {[0:pool_max_sz-1]};
        }

        foreach (share[i]) {
            share[i].id inside {[0:pool_max_sz-1]};
        }
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
endclass