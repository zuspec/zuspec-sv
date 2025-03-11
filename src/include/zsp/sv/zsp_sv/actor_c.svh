typedef class actor_base_c;
typedef class backend_api;
typedef class activity_c;
typedef class component_c;

class actor_c #(
    type Tapi=backend_api,
    type Tc=component_c,
    type Ta=activity_c) extends actor_base_c;
    `zsp_component_util(actor_c)
    Tapi        api;
    Tc          top;

    function new(Tapi api=null);
        super.new("<actor>", null, null);

        if (api == null) begin
            api = new();
        end
        this.api = api;

        top = new("pss_top", null, this);

    endfunction

    virtual function backend_api get_backend();
        return api;
    endfunction

    virtual function Tapi get_api();
        return api;
    endfunction

endclass
