typedef class action_type_c;

class component_type_c extends obj_type_c;
    component_type_c    super_t;
    action_type_c       actions[$];

    function new(string name, component_type_c super_t=null);
        super.new(name);
        if (super_t != null && super_t != component_c::get_type()) begin
            this.super_t = super_t;
        end
    endfunction

    function void add_action_type(action_type_c action);
        actions.push_back(action);
    endfunction

endclass

