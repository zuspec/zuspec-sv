typedef class object_ref_c;
typedef class action_c;

class action_handle_c #(type action_t=action_c) extends object_ref_c #(action_t);;

    virtual function obj_type_c get_obj_type();
        return action_t ::get_type();
    endfunction


endclass
