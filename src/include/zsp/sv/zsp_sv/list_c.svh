
class list_c #(type T=int);
    rand T       store[$];

    function T __at__(int idx);
        return store[idx];
    endfunction

    function T __getitem__(int idx);
        return store[idx];
    endfunction

    function void __setitem__(int idx, T item);
        store[idx] = item;
    endfunction

    function int size();
        return store.size();
    endfunction

    function void clear();
        store = {};
    endfunction

    function void delete(int idx);
        store.delete(idx);
    endfunction

    function void insert(int idx, T elem);
        store.insert(idx, elem);
    endfunction

    function void push_back(T item);
        store.push_back(item);
    endfunction

endclass