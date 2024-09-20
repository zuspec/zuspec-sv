`ifndef INCLUDED_ZSP_SV_MACROS_SVH
`define INCLUDED_ZSP_SV_MACROS_SVH

`define zsp_reg_type_offset(regtype, regname) regtype``::``inst(null).``regname``.``offset

`define zsp_reg_type_arr_offset(regtype, regname, index) regtype``::``inst(null).``regname``.``offsets[``index``]


`endif /* INCLUDED_ZSP_SV_MACROS_SVH */
