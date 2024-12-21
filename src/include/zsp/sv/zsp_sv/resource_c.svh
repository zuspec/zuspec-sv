
class resource_c;
    int     instance_id;
    bit     locked;
    int     shared;

    function new(int instance_id);
        this.instance_id = instance_id;
    endfunction

endclass

