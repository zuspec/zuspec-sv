
typedef class object_ref_base_c;
typedef class object_refcnt_c;

class object_ref_c #(type T=object_refcnt_c) extends object_ref_base_c;

    function new(T obj=null);
        super.new(obj);
    endfunction

    virtual function T get();
        T ret;
        $cast(ret, obj);
        return ret;
    endfunction

endclass