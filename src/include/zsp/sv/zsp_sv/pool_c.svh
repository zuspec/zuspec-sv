
typedef class component_c;

class pool_c;
    string          name;
    component_c     parent;

    function new(string name, component_c parent);
        this.name = name;
        this.parent = parent;
    endfunction


endclass