
typedef class actor_base_c;
typedef class executor_base_c;

class activity_ctxt_c;
    component_c         comp;
    activity_ctxt_c     parent;

//    activity_scope_c    scope_s[$];

    function new(activity_ctxt_c parent=null, component_c comp=null);
        this.parent = parent;
        this.comp = (comp != null)?comp:((parent!=null)?parent.comp:null);
    endfunction

    virtual function actor_base_c get_actor();
        return parent.get_actor();
    endfunction

    virtual function component_c get_component();
        return comp;
    endfunction

    virtual function executor_base_c get_executor();
        return comp.get_default_executor();
    endfunction

    virtual task add(activity_c activity);
        activity.run(this);
    endtask

    virtual task end_scope();
    endtask

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
