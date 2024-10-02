
class backend_api #(type BaseT=empty_t) extends BaseT;
    virtual task write64(bit[63:0] addr, bit[63:0] data);
        $display("Fatal: write64 not implemented");
        $finish;
    endtask
    virtual task write32(bit[63:0] addr, bit[31:0] data);
        $display("Fatal: write32 not implemented");
        $finish;
    endtask
    virtual task write16(bit[63:0] addr, bit[15:0] data);
        $display("Fatal: write16 not implemented");
        $finish;
    endtask
    virtual task write8(bit[63:0] addr, bit[7:0] data);
        $display("Fatal: write8 not implemented");
        $finish;
    endtask
    virtual task read64(output bit[63:0] data, input bit[63:0] addr);
        $display("Fatal: read64 not implemented");
        $finish;
    endtask
    virtual task read32(output bit[31:0] data, input bit[63:0] addr);
        $display("Fatal: read32 not implemented");
        $finish;
    endtask
    virtual task read16(output bit[15:0] data, input bit[63:0] addr);
        $display("Fatal: read16 not implemented");
        $finish;
    endtask
    virtual task read8(output bit[7:0] data, input bit[63:0] addr);
        $display("Fatal: read8 not implemented");
        $finish;
    endtask
    virtual function void message(string msg);
        $display(msg);
    endfunction
endclass
