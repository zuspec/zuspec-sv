
typedef class activity_c;

class activity_scope_c extends activity_c;
    activity_c      children[$];

    function new(actor_base_c actor, component_c parent_comp);
        super.new(actor, parent_comp);
    endfunction


endclass
