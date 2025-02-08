

package custom_api_pkg;
    import pss_types::*;
    `include "custom_api.svh"
endpackage


module top;
    import pss_top__Entry_pkg::*;
    import custom_api_pkg::*;

    initial begin
        automatic custom_api api = new();
        automatic pss_top__Entry entry;

        $display("--> new (custom_api)");
        entry = new(api);
        $display("<-- new (custom_api)");
        $display("--> run");
        entry.run();
        $display("<-- run");
        $finish;
    end

endmodule

