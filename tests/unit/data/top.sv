
package top_pkg;
    import pss_top__Entry_pkg::*;

`ifdef VERILATOR
    class pss_top__Entry_api_impl implements pss_top__Entry_prv::import_api;
`else
    class pss_top__Entry_api_impl implements pss_top__Entry_api;
`endif
        virtual task write64(bit[63:0] addr, bit[63:0] data);
            $display("RES: write64 0x%08x 0x%08x", addr, data);
        endtask
        virtual task write32(bit[63:0] addr, bit[31:0] data);
            $display("RES: write32 0x%08x 0x%08x", addr, data);
        endtask
        virtual task write16(bit[63:0] addr, bit[15:0] data);
            $display("RES: write16 0x%08x 0x%08x", addr, data);
        endtask
        virtual task write8(bit[63:0] addr, bit[7:0] data);
            $display("RES: write8 0x%08x 0x%08x", addr, data);
        endtask
        virtual task read64(output bit[63:0] data, input bit[63:0] addr);
        endtask
        virtual task read32(output bit[31:0] data, input bit[63:0] addr);
        endtask
        virtual task read16(output bit[15:0] data, input bit[63:0] addr);
        endtask
        virtual task read8(output bit[7:0] data, input bit[63:0] addr);
        endtask
    endclass

endpackage

module top;
    import pss_top__Entry_pkg::*;
    import top_pkg::*;

    initial begin
        automatic pss_top__Entry entry;
        automatic  pss_top__Entry_api_impl api_impl = new();

        entry = new(api_impl);
        entry.run();
        $finish;
    end

endmodule

