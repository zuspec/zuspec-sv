
class component_ctor_ctxt_c;
    actor_c         actor;
    executor_base   executor_m[];
    component_c     comp_inst_s[$];

    function new(actor_c actor, int n_executor_types);
        this.actor = actor;
        executor_m = new[n_executor_types+1];
    endfunction

    function executor_base get_default_executor();
        return actor.get_default_executor();
    endfunction

    function void enter(component_c comp);
        $display("enter %0s", comp.name);
        if (!comp_inst_s.size || comp_inst_s[comp_inst_s.size-1] != comp) begin
            obj_type_c comp_t = comp.get_obj_type();
            comp_inst_s.push_back(comp);
            for (int i=comp_inst_s.size-1; i>=0; i--) begin
                comp_inst_s[i].add_comp_inst(comp);
            end
        end
    endfunction

    function void leave(component_c comp);
        $display("leave %0s", comp.name);
        if (comp_inst_s.size && comp_inst_s[comp_inst_s.size-1] == comp) begin
            comp_inst_s.pop_back();
        end
    endfunction

endclass

// Temp
typedef component_ctor_ctxt_c component_ctor_ctxt;