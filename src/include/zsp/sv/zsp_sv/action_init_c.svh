
typedef class action_c;

class action_init_c #(type Ta=action_c);
    virtual function void initialize(Ta action);
        `ZSP_FATAL(("action_init_c::initialize: action is null"));
    endfunction
endclass
