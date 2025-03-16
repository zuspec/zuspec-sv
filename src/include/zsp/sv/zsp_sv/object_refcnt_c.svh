
typedef class object_mgr_c;

/**
 * Base class for all objects that support allocation
 */
class object_refcnt_c;
    object_mgr_c        mgr;
    int                 cnt;

    function new(object_mgr_c mgr=null);
        this.mgr = mgr;
    endfunction

    virtual function void inc();
        cnt += 1;
    endfunction

    virtual function void dec();
        if (cnt != 0) begin
            cnt -= 1;
            if (cnt == 0) begin
                // Object is no longer referenced
                drop();
            end
        end
    endfunction

    virtual function void drop();
        if (mgr != null) begin
            mgr.release_obj(this);
        end
    endfunction

endclass
