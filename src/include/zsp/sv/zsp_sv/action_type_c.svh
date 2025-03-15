
typedef class component_type_c;

class ref_claim_type_c;
    string      name;
    int         id;
    bit         is_claim;
    bit         is_lock;
    int         count;
    obj_type_c  obj_t;

    function new(
        string name,
        int id,
        bit is_claim,
        bit is_lock,
        int count,
        obj_type_c obj_t);
        this.name = name;
        this.id = id;
        this.is_claim = is_claim;
        this.is_lock = is_lock;
        this.count = count;
        this.obj_t = obj_t;
    endfunction

endclass

class action_type_c extends obj_type_c;
    ref_claim_type_c      ref_claim_type_l[$];
    int                   ref_claim_name_m[string];
    component_type_c      comp_t;

    function new(string name, component_type_c comp_t);
        super.new(name);
        this.comp_t = comp_t;
        comp_t.add_action_type(this);
    endfunction

    function int refid(string name);
        if (ref_claim_name_m.exists(name) != 0) begin
            return ref_claim_name_m[name];
        end else begin
            `ZSP_FATAL(("refid: ref_claim_name_m for %0s does not contain %0s", this.name, name));
            return -1;
        end
    endfunction

    function void add_ref(string name, obj_type_c obj_t, int count=1);
        ref_claim_type_c info = new(name, ref_claim_type_l.size(), 0, 0, count, obj_t);
        ref_claim_type_l.push_back(info);
        ref_claim_name_m[name] = info.id;
    endfunction

    function void add_lock(string name, obj_type_c obj_t, int count=1);
        ref_claim_type_c info = new(name, ref_claim_type_l.size(), 1, 1, count, obj_t);
        ref_claim_type_l.push_back(info);
        ref_claim_name_m[name] = info.id;
    endfunction

    function void add_share(string name, obj_type_c obj_t, int count=1);
        ref_claim_type_c info = new(name, ref_claim_type_l.size(), 1, 0, count, obj_t);
        ref_claim_type_l.push_back(info);
        ref_claim_name_m[name] = info.id;
    endfunction

    virtual function component_type_c get_comp_t();
        return comp_t;
    endfunction

    virtual function action_c mk();
        `ZSP_FATAL(("mk: action_type_c::mk is not implemented"));
        return null;
    endfunction

endclass

class action_type_t_c #(type Ta=action_c) extends action_type_c;
    bit ovr;

    function new(string name, component_type_c comp_t, bit ovr=0);
        super.new(name, comp_t);
        this.ovr = ovr;
    endfunction

    virtual function action_c mk();
        Ta action = new();
        return action;
    endfunction

endclass
