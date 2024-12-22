`ifndef INCLUDED_ZSP_SV_MACROS_SVH
`define INCLUDED_ZSP_SV_MACROS_SVH

`define zsp_reg_type_offset(regtype, regname) regtype``::``inst(null).``regname``.``offset

`define zsp_reg_type_arr_offset(regtype, regname, index) regtype``::``inst(null).``regname``.``offsets[``index``]

`define zsp_inc(obj) if (obj != null) obj .inc()

`define zsp_dec(obj) if (obj != null) obj .dec()

`define zsp_typed_obj_util(tname) \
    static obj_type_c __type = new(`"tname`"); \
    static function obj_type_c get_type(); \
        return __type; \
    endfunction \
    virtual function obj_type_c get_obj_type(); \
        return __type; \
    endfunction



`endif /* INCLUDED_ZSP_SV_MACROS_SVH */
