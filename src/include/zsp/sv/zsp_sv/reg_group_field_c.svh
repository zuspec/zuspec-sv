
class reg_group_field_c #(type group_t=reg_group_c) extends reg_group_field_base_c;
    group_t     group;

    function new(string name, group_t group);
        super.new(name);
        group = group;
    endfunction

    virtual function reg_group_c get_type();
        return group;
    endfunction
endclass

