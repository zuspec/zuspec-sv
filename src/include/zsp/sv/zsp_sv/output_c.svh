
typedef class object_c;
typedef class object_ref_c;

class output_c #(type T=object_c) extends object_ref_c #(T);

    function new();
        T obj = new();
        super.new(obj);
    endfunction

endclass
