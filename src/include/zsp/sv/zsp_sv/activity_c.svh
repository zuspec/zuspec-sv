
class activity_c extends object;
    actor_c             actor;  
    component_c         parent_comp;

    function new(actor_c actor, component_c parent_comp);
        this.actor = actor;
        this.parent_comp = parent_comp;
    endfunction

    virtual task run();
        $display("FATAL: activity::run not implemented");
        $finish;
    endtask

endclass