
// SystemVerilog classes do not use braces. The end the class declaration with a semicolon.
// SystemVerilog classes have virtual methods, but do not use '= 0' to indicate pure virtual methods.
class activity_seq_c : public activity_c;
    activity_c      sub_activities[$];

    function new(actor_c actor, component_c parent_comp);
        super.new(actor, parent_comp);
    endfunction

    task run();
        foreach (sub_activities[i]) begin
            sub_activities[i].run();
        end
    endtask

endclass

