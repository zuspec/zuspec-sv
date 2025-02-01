
class transparent_addr_space_c extends component_c;

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        super.new(name, ctxt, parent);
    endfunction

    function addr_handle_t add_nonallocatable_region(addr_region_s region);
        addr_handle_t ret = new(null, null, region.addr);
        return ret;
    endfunction

endclass
