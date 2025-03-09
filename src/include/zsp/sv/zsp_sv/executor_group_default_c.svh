
typedef class component_c;
typedef class component_ctor_ctxt;
typedef class executor_group_base_c;

class executor_group_default_c extends executor_group_base_c;
    `zsp_component_util(executor_group_default_c)

    function new(string name, component_ctor_ctxt ctxt, component_c parent);
        super.new(name, ctxt, parent);

        if (parent != null) begin
            if (parent.exec_group_default == null) begin
                parent.exec_group_default = this;
            end else begin
                `ZSP_FATAL(("parent already has a default executor group"));
            end
        end
    endfunction

    virtual function obj_type_c get_trait_type();
        return null;
    endfunction

endclass
