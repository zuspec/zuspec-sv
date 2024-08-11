
module top;
    import pss_top__Entry_pkg::*;

    initial begin
        automatic pss_top__Entry entry;

        entry = new();
        entry.run();
    end

endmodule

