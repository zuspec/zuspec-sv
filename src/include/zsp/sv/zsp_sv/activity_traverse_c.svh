
class activity_traverse_c extends activity_c;
    action_c        action;

    function new(actor_c actor, action);
        super.new(actor);
        this.action = action;
    endfunction

    task run();
        action.pre_solve();
        executor_base exec_b = actor.get_default_executor();
        // TODO: randomize action
        // TODO: connect up the selected component
        action.post_solve();

        // TODO: select the executor
        action.body(exec_b);
    endtask

endclass

