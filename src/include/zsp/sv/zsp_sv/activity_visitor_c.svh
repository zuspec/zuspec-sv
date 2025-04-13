
typedef class activity_traverse_base_c;
typedef class activity_type_c;
typedef class activity_scope_c;
typedef class activity_scope_par_c;

class activity_visitor_c;
    virtual function void visit_activity(activity_type_c t);
    endfunction

    virtual function void visit_scope(activity_scope_c t);
        visit_activity(t);
        foreach (t.children[i]) begin
            t.children[i].accept(this);
        end
    endfunction

    virtual function void visit_traverse(activity_traverse_base_c t);
        visit_activity(t);
    endfunction

    virtual function void visit_parallel(activity_scope_par_c t);
        visit_scope(t);
    endfunction

    virtual function void visit_parallel_branch(activity_scope_par_branch_c t);
        visit_sequence(t);
    endfunction

    virtual function void visit_sequence(activity_scope_seq_c t);
        visit_scope(t);
    endfunction

endclass
