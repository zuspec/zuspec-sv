
typedef class action_type_c;
typedef class activity_c;

class activity_traverse_base_c extends activity_c;
    rand action_c      action;
`ifdef VERILATOR
    // As of 5.029, Verilator does not support random queues
    action_constraint_base_c   constraints[$];
`else
    rand action_constraint_base_c   constraints[$];
`endif

    function new(actor_c actor, component_c parent_comp);
        super.new(actor, parent_comp);
    endfunction

    virtual function action_type_c get_action_type();
        `ZSP_FATAL(("get_action_type not implemented"));
        return null;
    endfunction

    virtual function void set_action(action_c action);
        this.action = action;
    endfunction

    function void add_constraint(action_constraint_base_c c);
        constraints.push_back(c);
    endfunction

    virtual task run_body(executor_base_c exec_b);
        `ZSP_FATAL(("run_body not implemented"));
    endtask

    task run();
        executor_base_c exec_b;
        executor_group_base_c exec_group;
        component_c comp;
        `ZSP_DEBUG_ENTER("activity_traverse_c", ("run"));

        action.init(actor, parent_comp);

        if (this.action.get_component() == null) begin
            // The context didn't assign component. Need to 
            // perform the context solving here...
            solve_action_context_c ctxt_solver = new(parent_comp);
`ifdef UNDEFINED
            void'(ctxt_solver.add_action(action));
            void'(ctxt_solver.resolve());
`endif // UNDEFINED
        end

        comp = this.action.get_component();
        exec_group = comp.get_executor_group();
        exec_b = exec_group.executors[0];

        `ZSP_DEBUG("activity_traverse_c", ("executor for comp %0s: %0s/%0s",
            comp.name,
            exec_group.name,
            exec_b.name));

        action.pre_solve();


        // Randomize action. Any traversals with an inline `with` 
        // get their own activity specialization
        // TODO: Handle errors
        if (!this.randomize()) begin
            `ZSP_FATAL(("activity_traverse_base_c::run failed to randomize"));
        end

        // TODO: connect up the selected component
        action.post_solve(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].enter_traverse(action);
        end

        run_body(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].leave_traverse(action);
        end

        `ZSP_DEBUG_LEAVE("activity_traverse_c", ("run"));
    endtask

endclass
