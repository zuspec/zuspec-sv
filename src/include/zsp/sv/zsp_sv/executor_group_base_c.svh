
class executor_group_base_c extends component_c;
    executor_base_c executors[$];

    function new(string name, component_ctor_ctxt_c ctxt, component_c parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function obj_type_c get_trait_type();
        `ZSP_FATAL(("get_trait_type not implemented"));
        return null;
    endfunction

    virtual function void add_executor(executor_base_c exec);
        executors.push_back(exec);
    endfunction

endclass
