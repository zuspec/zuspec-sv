
class addr_region_base_s extends object;
    bit[63:0]           size;
    string              tag;
    bit[63:0]           addr;

    virtual function bit[63:0] get_address();
        return addr;
    endfunction
endclass
