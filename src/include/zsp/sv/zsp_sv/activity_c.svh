
class activity_c extends object;
    actor_c             actor;  

    function new(actor_c actor);
        this.actor = actor;
    endfunction

    virtual task run();
    endtask

endclass