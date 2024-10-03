typedef class executor_base;

class addr_handle_t extends object_pool_base;
    executor_base       exec_b;
    storage_handle_s    base;
    bit[63:0]           offset;
    
    function new(
        executor_base       exec_b,
        storage_handle_s    base=null, 
        bit[63:0]           offset=0);
        this.exec_b = exec_b;
        this.base = base;
        this.offset = offset;
        if (base != null) begin
            base.inc();
        end
    endfunction

    virtual function void drop();
        actor_c actor = exec_b.get_actor();
        $display("addr_handle_t::drop");
        actor.addr_handle_drop(this);
        if (base != null) begin
            base.dec();
        end
    endfunction

    function addr_handle_t make_handle(bit[63:0] offset);
        addr_handle_t ret;
        if (this.base != null) begin
            ret = new(exec_b, this.base, this.offset+offset);
        end else begin
            ret = new(exec_b, null, this.offset+offset);
        end
        return ret;
    endfunction

    virtual function bit[63:0] addr_value();
        bit[63:0] ret = offset;
        if (this.base != null) begin
            ret += this.base.addr;
        end
        return ret;
    endfunction
endclass
