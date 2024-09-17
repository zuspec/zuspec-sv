
class list_c #(type T=int);
    T       m_store[$];

    function T __at__(int idx);
        return m_store[idx];
    endfunction

    function T __getitem__(int idx);
        return m_store[idx];
    endfunction

    function void __setitem__(int idx, T item);
        m_store[idx] = item;
    endfunction

    function int size();
        return m_store.size();
    endfunction

    function void clear();
        m_store = {};
    endfunction

    function void delete(int idx);
        m_store.delete(idx);
    endfunction

    function void insert(int idx, T elem);
        m_store.insert(idx, elem);
    endfunction

    function void push_back(T item);
        m_store.push_back(item);
    endfunction

endclass