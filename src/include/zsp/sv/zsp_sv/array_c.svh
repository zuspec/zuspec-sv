
class array_c #(type T=int, int SZ);
    rand T           store[SZ];
    
    function T __getitem__(int idx);
        return store[idx];
    endfunction

    function void __setitem__(int idx, T item);
        store[idx] = item;
    endfunction

    function int size();
        return SZ;
    endfunction

endclass