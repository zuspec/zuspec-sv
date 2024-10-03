
package zsp_sv;


typedef class object_pool_base;
typedef class actor_c;
typedef class component;
typedef class executor_base;

    `include "array_c.svh"
    `include "list_c.svh"

    typedef enum {NONE, LOW, MEDIUM, HIGH, FULL} message_verbosity_e;

class empty_t;
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

class object extends object_pool_base;

    virtual function void init(executor_base exec_b);
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



    `include "storage_handle_s.svh"
    `include "addr_handle_t.svh"



class addr_claim_t;
    storage_handle_s    storage;
endclass

function automatic addr_handle_t make_handle_from_claim(
        executor_base   exec_b,
        addr_claim_t    claim, 
        bit[63:0]       offset);
    addr_handle_t ret;
    ret = new(exec_b, claim.storage, offset);    
    return ret;
endfunction

function automatic addr_handle_t make_handle_from_handle(
    executor_base       exec_b,
    addr_handle_t       hndl,
    bit[63:0]           offset);
    addr_handle_t ret;
    if (hndl != null) begin
        if (hndl.base != null) begin
            ret = new(exec_b, hndl.base, hndl.offset+offset); 
        end else begin
            ret = new(exec_b, null, hndl.offset+offset); 
        end
    end else begin
        ret = new(exec_b, null, offset); 
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

    virtual function actor_c get_actor();
        component c = parent;
        actor_c actor;

        while (c.parent != null) begin
            c = c.parent;
        end
        $cast(actor, c);
        return actor;
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
        addr_handle_t ret = new(null, null, region.addr);
        $display("add_nonallocatable_region: 0x%08h %0d", region.addr, ret.count);
        return ret;
    endfunction
endclass

    typedef class reg_group_c;
    `include "reg_field_c.svh"
    `include "reg_group_c.svh"
    `include "reg_group_field_base_c.svh"
    `include "reg_group_field_arr_c.svh"
    `include "reg_group_field_c.svh"

    `include "hndl_drop_listener.svh"

    `include "backend_api.svh"
    `include "executor_base.svh"
    `include "actor_c.svh"

endpackage


