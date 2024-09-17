
class map_c #(type Tk=int, type Tv=int);
    Tv          m_store[Tk];

    virtual function Tv __getitem__(Tk key);
        return m_store[key];
    endfunction

    virtual function void __setitem__(Tk key, Tv val);
        m_store[key] = val;
    endfunction


endclass