
class component;
    int         comp_id;
    string      name;
    component   parent;

    executor_base  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        if (ctxt != null) begin
            this.comp_id = ctxt.actor.comp_l.size();
            ctxt.actor.comp_l.push_back(this);
        end else begin
            this.comp_id = -1;
        end
        this.name = name;
        this.parent = parent;
    endfunction

    virtual function void init_down(executor_base exec_b);
    endfunction

    virtual function void init(executor_base exec_b);
    endfunction

    virtual function void init_up(executor_base exec_b);
    endfunction

    virtual function void start(executor_base exec_b);
    endfunction

    virtual function bit check();
        return 1;
    endfunction

    virtual function actor_c get_actor();
        component c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor;
    endfunction

    virtual function executor_base get_default_executor();
        component c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor.get_default_executor();
    endfunction

endclass
