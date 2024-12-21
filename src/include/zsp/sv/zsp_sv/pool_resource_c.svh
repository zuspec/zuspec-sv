
typedef class resource_c;

class pool_resource_base_c extends pool_c;
    int         size;
    resource_c  resources[];

    function new(string name, component_c parent, int size);
        super.new(name, parent);
        this.size = size;
    endfunction

    // TODO: need to have pool participate in build process


endclass

class pool_resource_c #(type Tr=resource_c) extends pool_resource_base_c;

    function new(string name, component_c parent, int size);
        super.new(name, parent, size);
    endfunction

    function void build();
        resources = new[size];
        for (int i=0; i<size; i++) begin
            resources[i] = new(i);
        end
    endfunction

endclass


