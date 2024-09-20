
class reg_group_field_arr_c #(type group_t=reg_group_c) extends reg_group_field_base_c;
    group_t     group;
    bit[63:0]   offsets[];

    function new(string name, int sz, group_t group);
        super.new(name);
        offsets = new[sz];
        this.group = group;
//        group = group_t::inst(exec_b);
    endfunction

    virtual function reg_group_c get_type();
        return group;
    endfunction

    virtual function void map(
        executor_base   exec_b,
        reg_group_c     group);
        foreach (offsets[i]) begin
            offsets[i] = group.get_offset_of_instance_array(exec_b, name, i);
        end
    endfunction

endclass