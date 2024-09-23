
class executor_base extends component;
    actor_c            actor;
    backend_api        api;

    function new(string name, component parent);
        super.new(name, null, parent);
    endfunction

    function backend_api get_api();
        if (api == null) begin
            component c = this;
            actor_c actor;
            while (c.parent != null) begin
                c = c.parent;
            end
            if (!$cast(actor, c)) begin
                $display("Error: failed to cast root component to actor_c");
            end
            if (!$cast(api, actor.get_backend())) begin
                $display("Error: failed to cast api to api_t");
            end
        end
        return api;
    endfunction

    function actor_c get_actor();
        if (actor == null) begin
            component c = this;
            while (c.parent != null) begin
                c = c.parent;
            end
            if (!$cast(actor, c)) begin
                $display("Error: failed to cast root component to actor_c");
            end
        end
        return actor;
    endfunction

    virtual function bit[63:0] addr_value(addr_handle_t hndl);
        return hndl.addr_value();
    endfunction

    virtual task read8 (output bit[7:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read8(data, addr);
    endtask

    virtual task read16(output bit[15:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read16(data, addr);
    endtask

    virtual task read32(output bit[31:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read32(data, addr);
    endtask

    virtual task read64(output bit[63:0] data, input addr_handle_t hndl);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.read64(data, addr);
    endtask

    virtual task write8 (addr_handle_t hndl, bit[7:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write8(addr, data);
    endtask

    virtual task write16(addr_handle_t hndl, bit[15:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write16(addr, data);
    endtask

    virtual task write32(addr_handle_t hndl, bit[31:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write32(addr, data);
    endtask

    virtual task write64(addr_handle_t hndl, bit[63:0] data);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
        api.write64(addr, data);
    endtask

    virtual task read_bytes (addr_handle_t hndl, bit[7:0] data[$], int size);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

    virtual task write_bytes(addr_handle_t hndl, bit[7:0] data[$]);
        backend_api api = get_api();
        bit[63:0] addr = addr_value(hndl);
//        api.write64(addr, data);
    endtask

endclass
