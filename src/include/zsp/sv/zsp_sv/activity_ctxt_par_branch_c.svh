
typedef class activity_ctxt_c;
typedef class activity_ctxt_par_c;
//typedef class activity_visitor_c;
class activity_ctxt_par_branch_c extends activity_ctxt_c;
    activity_c                  activity;
    activity_traverse_base_c    first_traversal;

    function new(activity_c activity, activity_ctxt_c parent);
        super.new(parent);
        this.activity = activity;
    endfunction

    virtual task end_scope();
        activity_ctxt_par_c par;

        $cast(par, parent);

        // If this branch didn't have a first traversal, then
        // dummy out a response
        if (first_traversal == null) begin
            // Dummy-out a notification
            par.first_traversal(this);
        end

        // Notify that we're complete
        par.branch_complete(this);
    endtask

    virtual task enter_traverse(activity_traverse_base_c traverse);
        if (first_traversal == null) begin
            activity_ctxt_par_c par;
            $cast(par, parent);
            // This is the first traversal
            first_traversal = traverse;
            par.first_traversal(this);
        end
    endtask

    static function activity_ctxt_par_branch_c mk(activity_c activity, activity_ctxt_c parent);
        activity_ctxt_par_branch_c ret = new(activity, parent);
        return ret;
    endfunction

endclass

