
class channel_c #(type Te=int, int DEPTH=1) extends component_c;
    mailbox #(Te)       m_imp;

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        super.new(name, ctxt, parent);
        m_imp = new(DEPTH);
    endfunction

    task put(Te t);
        m_imp.put(t);
    endtask

    function bit try_put(Te t);
        return m_imp.try_put(t);
    end

    task get(output Te t);
        m_imp.get(t);
    endtask

    function Te try_get();
        Te ret;
        if (try_get(ret)) begin
            return ret;
        end else begin
            return null;
        end
    endfunction

endclass