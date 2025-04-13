
typedef class object_c;
typedef class object_ref_c;

class output_c #(type T=object_c) extends object_ref_c #(T);
    rand T obj;

    function new();
//        T obj = new();
        super.new(null);
    endfunction

    function void pre_randomize();
        $cast(obj, super.obj);
    endfunction

endclass
