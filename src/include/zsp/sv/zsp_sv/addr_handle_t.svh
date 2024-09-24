
class addr_handle_t extends object_pool_base;
    actor_c             actor;
    storage_handle_s    base;
    bit[63:0]           offset;
    
    function new(
        actor_c          actor,
        storage_handle_s base=null, 
        bit[63:0] offset=0);
        this.actor = actor;
        this.base = base;
        this.offset = offset;
        if (base != null) begin
            base.inc();
        end
    endfunction

    virtual function void drop();
        $display("addr_handle_t::drop");
        actor.addr_handle_drop(this);
        if (base != null) begin
            base.dec();
        end
    endfunction

    function addr_handle_t make_handle(bit[63:0] offset);
        addr_handle_t ret;
        if (this.base != null) begin
            ret = new(actor, this.base, this.offset+offset);
        end else begin
            ret = new(actor, null, this.offset+offset);
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
