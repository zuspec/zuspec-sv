
typedef class executor_base_c;
typedef class object_refcnt_c;


class object_c extends object_refcnt_c;
    virtual function void do_pre_solve(executor_base_c exec_b);
    endfunction

    virtual function void pre_solve(executor_base_c exec_b);
    endfunction

    virtual function void do_post_solve(executor_base_c exec_b);
    endfunction

    virtual function void post_solve(executor_base_c exec_b);
    endfunction
endclass
