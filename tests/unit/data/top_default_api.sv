

module top;
    import pss_top__Entry_pkg::*;

    initial begin
        automatic pss_top__Entry entry;

        $display("--> new");
        entry = new();
        $display("<-- new");
        $display("--> run");
        entry.run();
        $display("<-- run");
        $finish;
    end

endmodule

