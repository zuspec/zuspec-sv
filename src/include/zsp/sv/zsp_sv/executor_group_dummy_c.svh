
class executor_group_dummy_c extends executor_group_base_c;
    `zsp_typed_obj_util(executor_group_dummy_c)
    obj_type_c      trait_t;

    function new(
        string                  name,
        component_ctor_ctxt_c   ctxt,
        component_c             parent,
        obj_type_c              trait_t);
        super.new(name, ctxt, parent);
        this.trait_t = trait_t;
    endfunction

    virtual function obj_type_c get_trait_type();
        return trait_t;
    endfunction

endclass
