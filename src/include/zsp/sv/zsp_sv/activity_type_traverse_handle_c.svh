
typedef class activity_ctxt_c;
typedef class activity_type_c;
typedef class action_type_c;
typedef class action_handle_c;

class activity_type_traverse_handle_c #(type Ta=action_type_c) extends activity_type_c;
    action_handle_c #(Ta) action_h;

    function new(action_handle_c #(Ta) action_h, activity_ctxt_c ctxt);
        this.action_h = action_h;

        // TODO: add this handle to the active context
    endfunction

    function activity_c build(activity_ctxt_c ctxt);
        return this;
    endfunction

endclass

