
/**
 * An actor is a special type of component that autonomously 
 * runs behavior
 */
class actor_t_c #(type Tc=component, type Ta=int) extends Tc;
    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        super.new(name, ctxt, parent);
    endfunction

    task run();
//        Ta action = new();
//        activity_c #(Ta) activity = action.get_activity();

    endtask

    virtual function void start(executor_base exec_b);
        fork
            run_activity();
        join_none
    endfunction

endclass
