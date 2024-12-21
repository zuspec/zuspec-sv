
// Traversal constraints are added via aspects (?)

class activity_traverse_c #(type Ta, type Tact=activity_c) extends activity_c;
    rand Ta        action;

    // Core constraints:
    // - component id
    // - pool id

    function new(actor_c actor, Ta action=null);
        super.new(actor);
        if (action == null) begin
            action = new(actor);
        end
        this.action = action;
    endfunction

    task run();
        executor_base exec_b = actor.get_default_executor();
        action.pre_solve();

        // TODO: randomize action
        action.randomize();

        // TODO: connect up the selected component
        action.post_solve();

        // TODO: select the executor
        // TODO: how do we determine atomic vs compound?
        action.body(exec_b);

        begin
            Tact act = new(actor);
            act.run();
        end

    endtask

endclass

