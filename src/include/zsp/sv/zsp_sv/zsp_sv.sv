
package zsp_sv;


typedef class object_pool_base;
typedef class actor;
typedef class component;

class empty_t;
endclass

class object;
    object_pool_base    obj_pool;

    virtual function void init();
    endfunction

    virtual function void dtor();
    endfunction

    virtual function void do_pre_solve();
    endfunction

    virtual function void pre_solve();
    endfunction

    virtual function void do_post_solve();
    endfunction
    
    virtual function void post_solve();
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
    
    function new(addr_handle_t base, bit[63:0] offset);
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

class action extends object;

    virtual task body();
    endtask

endclass

class component;
    string      m_name;
    component   m_parent;

    function new(string name, component parent=null);
        m_name = name;
        m_parent = parent;
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

class actor #(
    type comp_t=component, 
    type activity_t=activity,
    type api_t=backend_api);
    comp_t      comp_tree;
    component   comp_l[$];
    api_t       api;
    // TODO: address-space

    function new(string name="");
        comp_tree = new(name);
    endfunction

    task run();
        activity_t root_activity = new();

        comp_tree.init();

        if (comp_tree.check()) begin
            root_activity.run();
        end else begin
            $display("Error: initialization check failed");
        end
    endtask

endclass

endpackage


