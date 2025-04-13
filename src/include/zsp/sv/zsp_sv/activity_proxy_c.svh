
typedef class activity_c;

class activity_proxy_c #(type T=activity_c) extends activity_c;
    T target;
    int id;

    function new(T target, int id=0);
        super.new();
        this.target = target;
        this.id = id;
    endfunction

    virtual task run(activity_ctxt_c ctxt, int id=0);
        target.run(ctxt, this.id);
    endtask

    static function activity_proxy_c #(T) mk(T target, int id);
        activity_proxy_c #(T) ret = new(target, id);
        return ret;
    endfunction

endclass

