
class hndl_drop_listener #(type T=int);

    virtual function void drop(T hndl);
    endfunction

endclass