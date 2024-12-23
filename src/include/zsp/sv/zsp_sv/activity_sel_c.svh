
class activity_sel_c extends activity_c;
    activity_c      sub_activities[$];

    function new(actor_c actor, component_c parent_comp);
        super.new(actor, parent_comp);
    endfunction

    virtual task run();
    endtask

endclass
