
package zsp_sv;


typedef class object_pool_base;
typedef class actor_c;
typedef class component;
typedef class executor_base;

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

    virtual function void do_post_solve(executor_base exec_b);
    endfunction
    
    virtual function void post_solve(executor_base exec_b);
    endfunction

endclass

interface class packed_s;

    pure virtual function bit[1023:0] pack();

    pure virtual function void pack_bytes(byte unsigned data[$]);

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

class activity_c extends object;
    virtual task run();
    endtask
endclass

class action extends object;
    rand bit[15:0]        parent_comp_id;
    rand bit[15:0]        comp_id;

    virtual task body(executor_base exec_b);
    endtask

    virtual function executor_base get_executor();
        return null;
    endfunction

endclass

    `include "activity_listener_c.svh"

class component_ctor_ctxt;
    actor_c        actor;
    executor_base  executor_m[];

    function new(actor_c actor, int n_executor_types);
        this.actor = actor;
        executor_m = new[n_executor_types+1];
    endfunction

    function executor_base get_default_executor();
        return actor.get_default_executor();
    endfunction

endclass

class component;
    int         comp_id;
    string      name;
    component   parent;

    executor_base  executor_m[];

    // aspace_t_map
    // executor_t_map

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        if (ctxt != null) begin
            this.comp_id = ctxt.actor.comp_l.size();
            ctxt.actor.comp_l.push_back(this);
        end else begin
            this.comp_id = -1;
        end
        this.name = name;
        this.parent = parent;
    endfunction

    virtual function void init_down(executor_base exec_b);
    endfunction

    virtual function void init(executor_base exec_b);
    endfunction

    virtual function void init_up(executor_base exec_b);
    endfunction

    virtual function bit check();
        return 1;
    endfunction

    virtual function executor_base get_default_executor();
        component c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor.get_default_executor();
    endfunction

endclass

class backend;
endclass

class addr_region_base_s extends object;
    bit[63:0]           size;
    string              tag;
    bit[63:0]           addr;

    virtual function bit[63:0] get_address();
        return addr;
    endfunction
endclass

class addr_space_c extends component;
    function new(string name, component_ctor_ctxt ctxt, component parent);
        super.new(name, ctxt, parent);
    endfunction

    virtual function addr_handle_t add_nonallocatable_region(addr_region_base_s region);
        addr_handle_t ret = new(null, region.addr);
        $display("add_nonallocatable_region: 0x%08h", region.addr);
        return ret;
    endfunction
endclass

class reg_field_c;
    string          name;
    bit[63:0]       offset;

    function new(string name);
        this.name = name;
    endfunction
endclass

class reg_field_arr_c extends reg_field_c;
    int             dim;

    function new(string name, int dim);
        super.new(name);
        this.dim = dim;
    endfunction
endclass

class reg_group_c;
    reg_field_c     fields[$];

    function new();
    endfunction


    function void map_registers(executor_base exec_b);
        $display("map_registers");
        foreach (fields[i]) begin
            reg_field_arr_c arr;

            if ($cast(arr, fields[i])) begin
                // TODO: handle arrayed registers
                $display("TODO: handle arrayed registers");
            end else begin
                fields[i].offset = get_offset_of_instance(exec_b, fields[i].name);
            end
            $display("reg: %0s %0d", fields[i].name, fields[i].offset);
        end
    endfunction

    virtual function bit[63:0] get_offset_of_instance(executor_base exec_b, string name);
        return {64{1'b1}};
    endfunction

    virtual function bit[63:0] get_offset_of_instance_array(executor_base exec_b, string name, int index);
        return {64{1'b1}};
    endfunction

endclass

class reg_group_field_base_c extends reg_field_c;

    function new(string name);
        super.new(name);
    endfunction

    function void set_handle(addr_handle_t hndl);
        offset = hndl.addr_value();
    endfunction

    virtual function reg_group_c get_type();
        return null;
    endfunction

endclass

class reg_group_field_c #(type group_t=reg_group_c) extends reg_group_field_base_c;
    group_t     group;

    function new(string name, executor_base exec_b);
        super.new(name);
        group = group_t::inst(exec_b);
    endfunction

    virtual function reg_group_c get_type();
        return group;
    endfunction
endclass

class backend_api #(type BaseT=empty_t) extends BaseT;
    virtual task write64(bit[63:0] addr, bit[63:0] data);
        $display("Fatal: write64 not implemented");
        $finish;
    endtask
    virtual task write32(bit[63:0] addr, bit[31:0] data);
        $display("Fatal: write32 not implemented");
        $finish;
    endtask
    virtual task write16(bit[63:0] addr, bit[15:0] data);
        $display("Fatal: write16 not implemented");
        $finish;
    endtask
    virtual task write8(bit[63:0] addr, bit[7:0] data);
        $display("Fatal: write8 not implemented");
        $finish;
    endtask
    virtual task read64(output bit[63:0] data, input bit[63:0] addr);
        $display("Fatal: read64 not implemented");
        $finish;
    endtask
    virtual task read32(output bit[31:0] data, input bit[63:0] addr);
        $display("Fatal: read32 not implemented");
        $finish;
    endtask
    virtual task read16(output bit[15:0] data, input bit[63:0] addr);
        $display("Fatal: read16 not implemented");
        $finish;
    endtask
    virtual task read8(output bit[7:0] data, input bit[63:0] addr);
        $display("Fatal: read8 not implemented");
        $finish;
    endtask
endclass

class executor_base extends component;
    backend_api        api;

    function new(string name, component parent);
        super.new(name, null, parent);
    endfunction

    function backend_api get_api();
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

    virtual function bit[63:0] addr_value(addr_handle_t hndl);
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

    virtual task write8 (addr_handle_t hndl, bit[7:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write8(addr, data);
    endtask

    virtual task write16(addr_handle_t hndl, bit[15:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write16(addr, data);
    endtask

    virtual task write32(addr_handle_t hndl, bit[31:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write32(addr, data);
    endtask

    virtual task write64(addr_handle_t hndl, bit[63:0] data);
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
    component               comp_l[$];
    activity_listener_c     listeners[$];
    // TODO: address-space

    function new(string name, component_ctor_ctxt ctxt, component parent=null);
        super.new(name, ctxt, null);
    endfunction

    virtual task run();
    endtask

    virtual function void add_listener(activity_listener_c listener);
        listeners.push_back(listener);
    endfunction

    virtual function backend_api get_backend();
        return null;
    endfunction

    virtual function executor_base get_default_executor();
        return null;
    endfunction

endclass

endpackage


