
typedef class activity_traverse_base_c;
typedef class solve_compset_c;

class solve_traversal_data_c;
    activity_traverse_base_c        activity;
    rand bit[31:0]                  comp_id;
    solve_compset_c                 compset;

    function new(activity_traverse_base_c activity, solve_compset_c compset);
        this.activity = activity;
        this.compset = compset;
    endfunction

    constraint comp_id_c {
        comp_id inside {compset.comp_id_l};
    }

endclass

