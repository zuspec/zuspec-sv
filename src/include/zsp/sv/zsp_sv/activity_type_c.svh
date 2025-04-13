typedef class activity_ctxt_c;
typedef class activity_type_visitor_c;

class activity_type_c;

    virtual function activity_c build(activity_ctxt_c ctxt);
        `ZSP_FATAL(("activity_type_c::build not implemented"));
        return null;
    endfunction

    virtual function void accept(activity_type_visitor_c v);
        v.visit_activity(this);
    endfunction

endclass