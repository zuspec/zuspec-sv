
class reg_field_c;
    string          name;
    bit[63:0]       offset;

    function new(string name);
        this.name = name;
    endfunction

    virtual function void map(
        executor_base   exec_b,
        reg_group_c     group);
        offset = group.get_offset_of_instance(exec_b, name);
    endfunction

endclass
