
class solve_resource_pool_ref_c extends solve_pool_ref_c;
    rand bit[31:0]          id;
    pool_resource_c         rpools[$];

    function void pre_randomize();
        foreach (pools[i]) begin
            pool_resource_c rpool;
            $cast(rpool, pools[i]);
            rpools.push_back(rpool);
        end
    endfunction

    constraint instance_id_c {
        foreach (rpools[i]) {
            (pool_id == i) -> (id < rpools[i].size);
        }
    }

endclass
