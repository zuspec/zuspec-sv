
typedef class action_c;
typedef class resource_c;

class resource_claim_base_c;
    int       ref_id; // Index within action type. Used to discover pool
    bit       lock;

    function new(string name, action_c parent, bit lock);
        this.lock = lock;
        ref_id = parent.add_resource_claim(this);
    endfunction

    virtual function void set(resource_c rsrc);
    endfunction

    virtual function obj_type_c get_type();
        $display("Fatal: get_type not implemented");
        return null;
    endfunction

endclass

class resource_claim_c #(type Tr=resource_c) extends resource_claim_base_c;
    rand Tr    rsrc;

    function new(string name, action_c parent, bit lock);
        super.new(name, parent, lock);
    endfunction

    function Tr get();
        Tr ret;
        if (!$cast(ret, rsrc)) begin
            $display("Fatal: bad cast");
        end
        return ret;
    endfunction

    virtual function void set(resource_c rsrc);
        if (!$cast(this.rsrc, rsrc)) begin
            $display("Fatal: bad cast");
        end
    endfunction

    virtual function obj_type_c get_type();
        return Tr::get_type();
    endfunction

endclass