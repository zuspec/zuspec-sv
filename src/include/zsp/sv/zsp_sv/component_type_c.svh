typedef class action_type_c;

class component_type_c extends obj_type_c;
    action_type_c    actions[$];

    function new(string name);
        super.new(name);
    endfunction

    function void add_action_type(action_type_c action);
        actions.push_back(action);
    endfunction

endclass

