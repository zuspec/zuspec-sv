
typedef class obj_type_c;

class typed_obj_c;

    virtual function obj_type_c get_obj_type();
        $display("FATAL: get_obj_type not implemented");
        $finish;
        return null;
    endfunction


    static function obj_type_c get_type();
        return null;
    endfunction

endclass
