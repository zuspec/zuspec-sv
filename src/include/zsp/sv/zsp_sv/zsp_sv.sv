
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
//    virtual function void release(object obj);
//    endfunction
endclass

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

endclass

class backend;
endclass

class actor #(type comp_t=component, type activity_t=activity);
    comp_t      comp_tree;
    component   comp_l[$];
    // TODO: address-space

    function new(string name="");
        comp_tree = new(name);
    endfunction

    task run();
        activity_t root_activity = new();

        comp_tree.init();

        root_activity.run();
    endtask

endclass

endpackage


