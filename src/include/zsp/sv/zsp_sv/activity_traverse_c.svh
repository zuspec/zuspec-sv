
// Traversal constraints are added via aspects (?)

typedef class action_constraint_base_c;

class activity_traverse_c #(type Ta, type Tact=activity_c) extends activity_c;
    rand Ta                         action;
`ifdef VERILATOR
    // As of 5.029, Verilator does not support random queues
    action_constraint_base_c   constraints[$];
`else
    rand action_constraint_base_c   constraints[$];
`endif

    // Core constraints:
    // - component id
    // - pool id

    function new(actor_c actor, component_c parent_comp, Ta action=null);
        super.new(actor, parent_comp);
        if (action == null) begin
            action = new();
        end
        this.action = action;
    endfunction

    function void add_constraint(action_constraint_base_c c);
        constraints.push_back(c);
    endfunction

    task run();
        executor_base exec_b = actor.get_default_executor();

        if (this.action.get_component() == null) begin
            // The context didn't assign component. Need to 
            // perform the context solving here...
            solve_action_context_c ctxt_solver = new(parent_comp);
            ctxt_solver.add_action(action);
            ctxt_solver.resolve();
        end

        action.pre_solve();


        // Randomize action. Any traversals with an inline `with` 
        // get their own activity specialization
        // TODO: Handle errors
        if (!do_randomize()) begin
            $display("FATAL: activity_traverse_c::run failed to randomize");
            $finish;
        end

        // TODO: connect up the selected component
        action.post_solve(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].enter_traverse(action);
        end

        // TODO: select the executor
        // Evaluate the action's body. The action decides whether to call
        // an exec body or a sub-activity
        action.body(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].leave_traverse(action);
        end

        // begin
        //     Tact act = new(actor);
        //     act.run();
        // end

    endtask

    function bit do_randomize();
        return action.randomize();
    endfunction

endclass

