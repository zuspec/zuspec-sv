
typedef class actor_c;

class activity_listener_c;

    virtual function void enter_actor(
        actor_c         actor
    );
    endfunction

    virtual function void leave_actor(
        actor_c         actor
    );
    endfunction

    virtual function void enter_traverse(
        action          action_h
    );
    endfunction

    virtual function void leave_traverse(
        action          action_h
    );
    endfunction

endclass
