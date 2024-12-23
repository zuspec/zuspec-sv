
class action_constraint_base_c;
endclass

class action_constraint_c #(type Ta=action_c) extends action_constraint_base_c;
    rand Ta         action;

    function new(Ta action);
        this.action = action;
    endfunction

endclass

// Define class between target <action> and parent <this>

