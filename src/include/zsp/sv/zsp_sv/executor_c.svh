

class executor_c #(type TRAIT = empty_executor_trait_s) extends executor_base_c;
    TRAIT   trait;

    virtual function obj_type_c get_trait_type();
        return TRAIT ::get_type();
    endfunction


endclass
