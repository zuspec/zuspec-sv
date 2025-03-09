
/**
 * Holds the valid set of component contexts 
 * that are available for a given action type
 */
class solve_compset_c;
    bit[31:0]            comp_id_l[$];
    component_c          comp_l[$];

    function void add_comp(component_c comp, int comp_id);
        comp_l.push_back(comp);
        comp_id_l.push_back(comp_id);
    endfunction

endclass