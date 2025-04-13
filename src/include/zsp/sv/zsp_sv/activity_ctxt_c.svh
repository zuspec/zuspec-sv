
typedef class actor_base_c;

class activity_ctxt_c;
    actor_base_c        actor;
    component_c         comp;
    activity_ctxt_c     parent;

//    activity_scope_c    scope_s[$];

    virtual function actor_base_c get_actor();
        return this.actor;
    endfunction

    virtual function component_c get_component();
        return comp;
    endfunction

    virtual task enter_activity(activity_c activity);
    endtask

    virtual task enter_traverse(activity_traverse_base_c traverse);
    endtask

    virtual task leave_traverse(activity_traverse_base_c traverse);
    endtask

    virtual task leave_activity(activity_c activity);
    endtask

/*
    virtual function void push_sequence();
    endfunction

    virtual function void push_parallel();
    endfunction

    virtual function void push_parallel_branch();
    endfunction

    virtual function void pop_parallel_branch();
    endfunction
 */


endclass
