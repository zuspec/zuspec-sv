
typedef class activity_ctxt_c;

class activity_ctxt_root_c extends activity_ctxt_c;
    actor_base_c        actor;

    function new(actor_base_c actor, component_c comp);
        super.new();
        this.actor = actor;
        this.comp = comp;
    endfunction

    virtual function actor_base_c get_actor();
        return this.actor;
    endfunction

endclass
