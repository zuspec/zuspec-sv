
class executor_group_base_c;
    executor_base_c executors[$];

    function new(string name, component_c parent);
        super.new(name, null, parent);

        if (parent != null) begin
            parent.exec_groups.push_back(this);
        end
    endfunction

    virtual function obj_type_c get_trait_type();
        `ZSP_FATAL(("get_trait_type not implemented"));
        return null;
    endfunction

    virtual function void add_executor(executor_base_c exec);
        executors.push_back(exec);
    endfunction

endclass
