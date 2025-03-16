
typedef class action_type_c;
typedef class activity_c;

class activity_traverse_base_c extends activity_c;
    rand action_c      action;
    `zsp_rand_arr action_constraint_base_c action_constraints[$];

    function new(actor_base_c actor, component_c parent_comp);
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
        action_constraints.push_back(c);
    endfunction

    virtual task run_body(executor_base_c exec_b);
        `ZSP_FATAL(("run_body not implemented"));
    endtask

    task run();
        executor_base_c exec_b;
        executor_group_base_c exec_group;
        component_c comp;
        `ZSP_DEBUG_ENTER("activity_traverse_c", ("run"));

        if (action == null) begin
            // The context didn't assign component. Need to 
            // perform the context solving here...
            solve_action_context_c ctxt_solver = new(parent_comp);
            `ZSP_DEBUG("activity_traverse_c", ("building/solving action, since the context did not"));
            void'(ctxt_solver.add_traversal(this));

            // Resolve calls us back to set the action handle
            void'(ctxt_solver.resolve());
        end

//        action.init(actor, parent_comp);

        comp = this.action.get_component();
        exec_group = comp.get_executor_group();
        exec_b = exec_group.executors[0];

        `ZSP_DEBUG("activity_traverse_c", ("executor for comp %0s: %0s/%0s",
            comp.name,
            exec_group.name,
            exec_b.name));

        action.pre_solve(exec_b);

        foreach (action_constraints[i]) begin
            action_constraints[i].action = action;
            action.layered_constraints.push_back(action_constraints[i]);
        end


        // Randomize action. Any traversals with an inline `with` 
        // get their own activity specialization
        // TODO: Handle errors
        if (this.randomize() == 0) begin
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
