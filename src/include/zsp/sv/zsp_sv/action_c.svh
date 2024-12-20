
class action_c extends object;
    rand bit[15:0]        parent_comp_id;
    rand bit[15:0]        comp_id;

    virtual task body(executor_base exec_b);
    endtask

    virtual function executor_base get_executor();
        return null;
    endfunction

endclass
