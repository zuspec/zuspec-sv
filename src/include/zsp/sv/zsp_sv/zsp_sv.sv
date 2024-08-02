
package zsp_sv;


typedef class object_pool_base;
typedef class actor;
typedef class component;

class object;
    object_pool_base    obj_pool;

    virtual function void init();
    endfunction

    virtual function void dtor();
    endfunction

    virtual function void pre_solve();
    endfunction
    
    virtual function void post_solve();
    endfunction

endclass

class object_pool_base;
//    virtual function void release(object obj);
//    endfunction
endclass

class action extends object;

    virtual task body();
    endtask

    virtual task activity();
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

endclass

class backend;
endclass

class actor #(type comp_t=component, type action_t=action);
    comp_t      comp_tree;
    component   comp_l;
    // TODO: address-space

    function new(string name="");
        comp_tree = new(name);
    endfunction

    task run();
        comp_tree.init();
        /*
        action_t    root_action = new();

        root_action.run(this);
         */
    endtask

endclass

endpackage


