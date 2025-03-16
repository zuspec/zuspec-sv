
typedef class object_refcnt_c;

/**
 * Base class for obect-reference types
 */
class object_ref_base_c;
    object_refcnt_c         obj;

    function new(object_refcnt_c obj);
        this.obj = obj;

        if (obj != null) begin
            obj.inc();
        end
    endfunction

    virtual function void set(object_ref_base_c rhs);
        if (this.obj != rhs.obj) begin
            if (rhs.obj != null) begin
                rhs.obj.inc();
            end

            if (this.obj != null) begin
                this.obj.dec();
            end

            this.obj = rhs.obj;
        end
    endfunction

    virtual function void drop();
        if (obj != null) begin
            obj.dec();
            obj = null;
        end
    endfunction

endclass
