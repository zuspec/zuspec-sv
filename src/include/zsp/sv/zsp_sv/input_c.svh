
typedef class object_c;
typedef class object_ref_c;

class input_c #(type T=object_c) extends object_ref_c #(T);
    rand T obj;

    function void pre_randomize();
        $cast(obj, super.obj);
    endfunction

    function new();
        super.new(null);
    endfunction


endclass
