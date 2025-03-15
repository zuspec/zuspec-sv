typedef class action_c;
typedef class action_type_c;

class component_type_c extends obj_type_c;
    component_type_c    super_t;
    action_type_c       actions[$];
    action_type_c       action_type_m[action_type_c];
    bit                 action_type_m_init;

    function new(string name, component_type_c super_t=null);
        super.new(name);
        if (super_t != null && super_t != component_c::get_type()) begin
            this.super_t = super_t;
        end
    endfunction

    function void add_action_type(action_type_c action);
        actions.push_back(action);
    endfunction

    virtual function action_c mk_action(action_type_c action_t);
        action_c action;
        `ZSP_DEBUG_ENTER("component_type_c", ("mk_action %0s: %0s", name, action_t.name));
        if (!action_type_m_init) begin
            component_type_c comp_t = super_t;

            // If base-r action matches an action name,
            // // alias that to the leaf-ier action
            // while (comp_t != null) begin
            //     for (int i=0; i<comp_t.actions.size(); i++) begin
            //         for (int j=0; j<actions.size(); j++) begin
            //             if (actions[j].name == comp_t.actions[i].name) begin
            //                 action_type_m[comp_t.actions[i]] = actions[j];
            //             end
            //         end
            //         action_type_m[comp_t.actions[i]] = comp_t.actions[i];
            //     end

            //     comp_t = comp_t.super_t;
            // end

            action_type_m_init = 1;
        end

        if (action_type_m.exists(action_t) != 0) begin
            `ZSP_DEBUG("component_type_c", ("using action_t %0s", action_type_m[action_t].name));
            action = action_type_m[action_t].mk();
        end else begin
            `ZSP_DEBUG("component_type_c", ("using action_t (self) %0s", action_t.name));
            action = action_t.mk();
        end

        action.set_component(null);

        `ZSP_DEBUG_LEAVE("component_type_c", ("mk_action %0s: %0s", name, action_t.name));
        return action;
    endfunction

endclass

