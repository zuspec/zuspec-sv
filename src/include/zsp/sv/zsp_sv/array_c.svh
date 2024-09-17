
class array_c #(type T=int, int SZ);
    T           m_store[SZ];
    
    function T __getitem__(int idx);
        return m_store[idx];
    endfunction

    function void __setitem__(int idx, T item);
        m_store[idx] = item;
    endfunction

    function int size();
        return SZ;
    endfunction

endclass