
typedef class empty_executor_trait_s;

class executor_c #(type TRAIT = empty_executor_trait_s) extends executor_base_c;
    `zsp_component_util(executor_c)
    TRAIT   trait;

    function new(string name, component_ctor_ctxt_c ctxt, component_c parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function obj_type_c get_trait_type();
        return TRAIT ::get_type();
    endfunction


endclass
