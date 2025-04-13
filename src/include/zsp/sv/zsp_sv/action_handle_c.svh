typedef class object_ref_c;
typedef class action_c;
typedef class action_handle_base_c;

class action_handle_c #(type action_t=action_c) extends action_handle_base_c;
    action_t                action;
    // As of 5.029, verilator doesn't like an inline call to ::get_type()
    static action_type_c    act_type = action_t ::get_type();

    // Need bind maps here...
    // Map of ref_id -> 
    // - list of outputs bound
    // - list of inputs bound

    function new(string name);
        super.new(name, act_type);
    endfunction

    function void bind_static(component_c comp);
        if (action == null) begin
            `ZSP_FATAL(("action_handle_c::bind_static: action is null"));
        end
        this.action = action;
        this.obj = action;
    endfunction

    virtual function obj_type_c get_obj_type();
        return action_t ::get_type();
    endfunction

    virtual function ref_claim_type_c getref(int id);
        action_type_c act_type = action_t ::get_type();

        return act_type.ref_claim_type_l[id];
    endfunction

endclass
