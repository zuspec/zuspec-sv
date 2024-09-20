
class reg_group_field_base_c extends reg_field_c;

    function new(string name);
        super.new(name);
    endfunction

    function void set_handle(addr_handle_t hndl);
        offset = hndl.addr_value();
    endfunction

    virtual function reg_group_c get_type();
        return null;
    endfunction

endclass
