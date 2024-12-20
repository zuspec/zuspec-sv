
package resource_pkg;
  class A_data;

    function void print();
    endfunction
  endclass

  class B_data;
    rand bit[7:0]  comp_id;
    rand bit[7:0]  pcomp_id;
    rand bit[7:0]  r_pool_id;
    rand bit[7:0]  r_inst_id;

    constraint comp_c {
        (pcomp_id == 0) -> comp_id inside {[1:2]};
        (comp_id == 1) -> r_pool_id == 0;
        (comp_id == 2) -> r_pool_id == 1;
        r_inst_id inside {[0:1]};
    }
    function void print();
        $display("B: comp_id=%0d pcomp_id=%0d r_pool_id=%0d r_inst_id=%0d",
            comp_id, pcomp_id, r_pool_id, r_inst_id);
    endfunction
  endclass

  class C_data;
    rand bit[7:0]  comp_id;
    rand bit[7:0]  pcomp_id;
    rand bit[7:0]  r_pool_id;
    rand bit[7:0]  r_inst_id;

    constraint comp_c {
        (pcomp_id == 0) -> comp_id inside {[1:2]};
        (comp_id == 1) -> r_pool_id == 0;
        (comp_id == 2) -> r_pool_id == 1;
        r_inst_id inside {[0:1]};
    }
    function void print();
        $display("C: comp_id=%0d pcomp_id=%0d r_pool_id=%0d r_inst_id=%0d",
            comp_id, pcomp_id, r_pool_id, r_inst_id);
    endfunction
  endclass

  class resource;
    rand bit[7:0]  A_comp_id;
    rand bit[7:0]  A_pcomp_id;
    rand bit[7:0]  A_r_pool_id;
    rand bit[7:0]  A_r_inst_id;

    constraint A_comp_c {
        (A_pcomp_id == 0) -> A_comp_id inside {[1:2]};
        (A_comp_id == 1) -> A_r_pool_id == 0;
        (A_comp_id == 2) -> A_r_pool_id == 1;
        A_r_inst_id inside {[0:1]};
    }

    rand bit[7:0]  B_comp_id;
    rand bit[7:0]  B_pcomp_id;
    rand bit[7:0]  B_r_pool_id;
    rand bit[7:0]  B_r_inst_id;

    constraint B_comp_c {
        (B_pcomp_id == 0) -> B_comp_id inside {[1:2]};
        (B_comp_id == 1) -> B_r_pool_id == 0;
        (B_comp_id == 2) -> B_r_pool_id == 1;
        B_r_inst_id inside {[0:1]};
    }

    rand bit[7:0]  C_comp_id;
    rand bit[7:0]  C_pcomp_id;
    rand bit[7:0]  C_r_pool_id;
    rand bit[7:0]  C_r_inst_id;

    constraint C_comp_c {
        (C_pcomp_id == 0) -> C_comp_id inside {[1:2]};
        (C_comp_id == 1) -> C_r_pool_id == 0;
        (C_comp_id == 2) -> C_r_pool_id == 1;
        C_r_inst_id inside {[0:1]};
    }

    constraint rsrc_c {
        A_pcomp_id == 0;
        B_pcomp_id == 0;
        C_pcomp_id == 0;

        // A.r != B.r ; A.r != C.r
        !(A_r_inst_id == B_r_inst_id && A_r_pool_id == B_r_pool_id);
        !(A_r_inst_id == C_r_inst_id && A_r_pool_id == C_r_pool_id);

    }

    function void print();
        $display("A: comp_id=%0d pcomp_id=%0d r_pool_id=%0d r_inst_id=%0d",
            A_comp_id, A_pcomp_id, A_r_pool_id, A_r_inst_id);
        $display("B: comp_id=%0d pcomp_id=%0d r_pool_id=%0d r_inst_id=%0d",
            B_comp_id, B_pcomp_id, B_r_pool_id, B_r_inst_id);
        $display("C: comp_id=%0d pcomp_id=%0d r_pool_id=%0d r_inst_id=%0d",
            C_comp_id, C_pcomp_id, C_r_pool_id, C_r_inst_id);
    endfunction
    
  endclass

endpackage

module top;
  import resource_pkg::*;

  initial begin
    automatic resource r = new();
    if (!r.randomize()) begin
        $display("FAIL");
    end else begin
        r.print();
    end
    $finish;
  end
endmodule