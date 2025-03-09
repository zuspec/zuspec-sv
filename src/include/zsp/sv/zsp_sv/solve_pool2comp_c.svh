
typedef class solve_pool_ref_c;
typedef class solve_pool2comp_map_c;
typedef class solve_traversal_data_c;

class solve_pool2comp_c;
    rand solve_pool_ref_c           pool_ref;
    rand solve_traversal_data_c     traversal_data;
    solve_pool2comp_map_c           pool2comp_map;

    constraint comp_id_c {
        foreach (pool2comp_map.pool2comp_m[i]) {
            (i == pool_ref.pool_id) -> (traversal_data.comp_id == pool2comp_map.pool2comp_m[i]);
        }
    }

endclass
