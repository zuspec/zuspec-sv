
class executor_group_c #(type TRAIT = empty_executor_trait_s) extends executor_group_base_c;

    virtual function obj_type_c get_trait_type();
        return TRAIT ::get_type();
    endfunction

endclass
