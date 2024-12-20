
class actor_c extends component;
    component                               comp_l[$];
    activity_listener_c                     listeners[$];
    hndl_drop_listener #(addr_handle_t)     addr_handle_drop_listeners[$];
    message_verbosity_e                     verbosity = MEDIUM;
    // TODO: address-space

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        super.new(name, ctxt, null);
    endfunction

    virtual task run();
        executor_base exec_b = get_default_executor();
        foreach (comp_l[i]) begin
            comp_l[i].start(exec_b);
        end
    endtask

    virtual function void start(executor_base exec_b);
        // The root actor's behavior is solely controlled from 'run'
    endfunction

    virtual function void add_listener(activity_listener_c listener);
        listeners.push_back(listener);
    endfunction

    virtual function void add_addr_handle_drop_listener(
        hndl_drop_listener #(addr_handle_t) l);
        addr_handle_drop_listeners.push_back(l);
    endfunction

    virtual function void addr_handle_drop(addr_handle_t hndl);
        foreach (addr_handle_drop_listeners[i]) begin
            addr_handle_drop_listeners[i].drop(hndl);
        end
    endfunction

    virtual function backend_api get_backend();
        return null;
    endfunction

    virtual function executor_base get_default_executor();
        return null;
    endfunction

endclass
