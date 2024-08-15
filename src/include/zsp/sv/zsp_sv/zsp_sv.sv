
package zsp_sv;


typedef class object_pool_base;
typedef class actor_c;
typedef class component;
typedef class executor_base;

class empty_t;
endclass

class object #(type executor_t=executor_base);
    object_pool_base    obj_pool;

    virtual function void init();
    endfunction

    virtual function void dtor();
    endfunction

    virtual function void do_pre_solve();
    endfunction

    virtual function void pre_solve();
    endfunction

    virtual function void do_post_solve(executor_t executor);
    endfunction
    
    virtual function void post_solve(executor_t executor);
    endfunction

endclass

class object_pool_base;
    int     count;

    virtual function void inc();
        count += 1;
    endfunction

    virtual function void dec();
        if (count) begin
            count -= 1;
            if (!count) begin
                // Object is no longer referenced
                drop();
            end
        end
    endfunction

    virtual function void drop();
    endfunction

//    virtual function void release(object obj);
//    endfunction
endclass


class addr_handle_t extends object_pool_base;
    addr_handle_t       base;
    bit[63:0]           offset;
    
    function new(addr_handle_t base=null, bit[63:0] offset=0);
        this.base = base;
        this.offset = offset;
        if (base != null) begin
            base.inc();
        end
    endfunction

    virtual function void dec();
        if (base != null) begin
            base.dec();
        end
    endfunction

    function addr_handle_t make_handle(bit[63:0] offset);
        addr_handle_t ret;
        if (this.base != null) begin
            ret = new(this.base, this.offset+offset);
        end else begin
            ret = new(this, this.offset+offset);
        end
        return ret;
    endfunction

    virtual function bit[63:0] addr_value();
        bit[63:0] ret = offset;
        if (this.base != null) begin
            ret += this.base.offset;
        end
        return ret;
    endfunction
endclass

class addr_claim_t;
    addr_handle_t       addr;
endclass

function automatic addr_handle_t make_handle_from_claim(
        addr_claim_t    claim, 
        bit[63:0]       offset);
    addr_handle_t ret;
    ret = new(claim.addr, offset);    
    return ret;
endfunction

function automatic addr_handle_t make_handle_from_handle(
        addr_handle_t   hndl,
        bit[63:0]       offset);
    addr_handle_t ret;
    if (hndl != null) begin
        if (hndl.base != null) begin
            ret = new(hndl.base, hndl.offset+offset); 
        end else begin
            ret = new(hndl, hndl.offset+offset); 
        end
    end else begin
        ret = new(null, offset); 
    end
    return ret;
endfunction

class activity extends object;
    virtual task run();
    endtask
endclass

class action #(type executor_t=executor_base) extends object #(executor_t);

    virtual task body(executor_t executor);
    endtask

    virtual function executor_t get_executor();
    endfunction

endclass

class component_ctor_ctxt;
    executor_base  executor_m[];

    function new(int n_executor_types);
        executor_m = new[n_executor_types+1];
    endfunction

endclass

class component;
    string      name;
    component   parent;

    executor_base  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        this.name = name;
        this.parent = parent;
    endfunction

    virtual function void init_down();
    endfunction

    virtual function void init();
    endfunction

    virtual function void init_up();
    endfunction

    virtual function bit check();
        return 1;
    endfunction

endclass

class backend;
endclass

class addr_region_s;
endclass

class addr_space_c extends component;
    function new(string name, component_ctor_ctxt ctxt, component parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function addr_handle_t add_nonallocatable_region(addr_region_s region);
    endfunction
endclass

interface class backend_api;
    pure virtual task write64(bit[63:0] addr, bit[63:0] data);
    pure virtual task write32(bit[63:0] addr, bit[31:0] data);
    pure virtual task write16(bit[63:0] addr, bit[15:0] data);
    pure virtual task write8(bit[63:0] addr, bit[7:0] data);
    pure virtual task read64(output bit[63:0] data, input bit[63:0] addr);
    pure virtual task read32(output bit[31:0] data, input bit[63:0] addr);
    pure virtual task read16(output bit[15:0] data, input bit[63:0] addr);
    pure virtual task read8(output bit[7:0] data, input bit[63:0] addr);
endclass

class executor_base #(type api_t=backend_api) extends component;
    actor_c           actor_h;
    api_t             api;

    function new(string name, component_ctor_ctxt ctxt, component parent);
        super.new(name, ctxt, parent);
//        this.actor_h = actor_h;
    endfunction

    function api_t get_api();
        if (api == null) begin
            component c = this;
            actor_c actor;
            while (c.parent != null) begin
                c = c.parent;
            end
            if (!$cast(actor, c)) begin
                $display("Error: failed to cast root component to actor_c");
            end
            if (!$cast(api, actor.get_backend())) begin
                $display("Error: failed to cast api to api_t");
            end
        end
        return api;
    endfunction

    virtual function bit[64] addr_value(addr_handle_t hndl);
        return hndl.addr_value();
    endfunction

    virtual task read8 (output bit[7:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read8(data, addr);
    endtask

    virtual task read16(output bit[15:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read16(data, addr);
    endtask

    virtual task read32(output bit[31:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read32(data, addr);
    endtask

    virtual task read64(output bit[63:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read64(data, addr);
    endtask

    virtual task write8 (addr_handle_t hndl, bit[8] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write8(addr, data);
    endtask

    virtual task write16(addr_handle_t hndl, bit[16] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write16(addr, data);
    endtask

    virtual task write32(addr_handle_t hndl, bit[32] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write32(addr, data);
    endtask

    virtual task write64(addr_handle_t hndl, bit[64] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write64(addr, data);
    endtask

    virtual task read_bytes (addr_handle_t hndl, bit[7:0] data[$], int size);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

    virtual task write_bytes(addr_handle_t hndl, bit[7:0] data[$]);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

endclass

class actor_c extends component;
    component   comp_l[$];
    // TODO: address-space

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        super.new(name, ctxt, null);
    endfunction

    virtual task run();
    endtask

    virtual function backend_api get_backend();
        return null;
    endfunction

    virtual function executor_base get_default_executor();
    endfunction

endclass

endpackage


