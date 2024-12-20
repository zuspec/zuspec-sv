
class activity_sel_c extends activity_c;
    activity_c      sub_activities[$];

    function new(actor_c actor);
        super.new(actor);
    endfunction

    virtual task run();
    endtask

endclass
