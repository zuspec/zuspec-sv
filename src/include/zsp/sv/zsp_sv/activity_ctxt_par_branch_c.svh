
typedef class activity_ctxt_c;
typedef class activity_ctxt_par_c;
//typedef class activity_visitor_c;
class activity_ctxt_par_branch_c extends activity_ctxt_c;
    activity_ctxt_par_c         parent;
    activity_c                  activity;
    activity_traverse_base_c    first_traversal;

    function new(activity_c activity);
        this.activity = activity;
    endfunction

    virtual task run(activity_ctxt_c ctxt);

        $cast(parent, ctxt);

        // Start the branch
        this.activity.run(this);

        if (first_traversal == null) begin
            // Dummy-out a notification
            parent.first_traversal(this);
        end

        // Notify that we're complete
        parent.branch_complete(this);
    endtask

    virtual task enter_traverse(activity_traverse_base_c traverse);
        if (first_traversal == null) begin
            // This is the first traversal
            first_traversal = traverse;
            parent.first_traversal(this);
        end
    endtask

//    virtual function void accept(activity_visitor_c v);
//        v.visit_parallel_branch(this);
//    endfunction

    static function activity_ctxt_par_branch_c mk(activity_c activity);
        activity_ctxt_par_branch_c ret = new(activity);
        return ret;
    endfunction

endclass

