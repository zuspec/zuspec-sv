
class executor_group_c #(type TRAIT = empty_executor_trait_s) extends executor_group_base_c;
    `zsp_typed_obj_util(executor_group_c)

    function new(string name, component_ctor_ctxt_c ctxt, component_c parent);
        super.new(name, ctxt, parent);

        if (parent != null) begin
            parent.exec_groups.push_back(this);
        end

    endfunction

    virtual function obj_type_c get_trait_type();
        return TRAIT ::get_type();
    endfunction

endclass
