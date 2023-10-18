
package zuspec;

  typedef class Backend;

  class Actor;
    chandle m_hndl;

    function new(
        string      comp_t,
        string      action_t,
        Backend     backend);
        process p = process::self();
        string randstate = p.get_randstate();

        m_hndl = zuspec_Actor_new(
            randstate,
            comp_t, 
            action_t,
            backend.m_hndl);

        if (m_hndl == null) begin
            $display("FATAL: zuspec_Actor_new returned null");
            $finish();
        end
    endfunction

    task run();
    endtask

  endclass

  class ValRef;
    chandle         m_hndl;

    function new(chandle hndl);
        m_hndl = hndl;
    endfunction
  endclass

  class EvalThread;
    chandle         m_hndl;

    function new(chandle hndl);
        m_hndl = hndl;
    endfunction

  endclass

  class Backend;
    static Backend proxy2backend_m[longint unsigned];
    longint unsigned m_hndl;

    function new();
        m_hndl = zuspec_EvalBackendProxy_new();
        proxy2backend_m[m_hndl] = this;
    endfunction

    virtual function void callFuncReq(
        EvalThread      thread,
        int             func_id,
        ValRef          params[]);
        $display("FATAL: zuspec::Backend::callFuncReq not implemented");
        $finish();
    endfunction

    virtual function void emitMessage(string msg);
        $display("FATAL: zuspec::Backend::emitMessage not implemented");
        $finish();
    endfunction

  endclass

  bit _init = pkg_init();
  function bit pkg_init();
    automatic string pss_files;
    automatic int load = 0;
    automatic process p = process::self();

    $display("randstate: %0s", p.get_randstate());
    if ($value$plusargs("zuspec.pssfiles=%s", pss_files)) begin
        $display("File: %0s", pss_files);
    end
    if ($value$plusargs("zuspec.load=%d", load)) begin
    end

    if (zuspec_init(pss_files, load) != 1) begin
        $display("FATAL: Failed to initialize Zuspec package");
        $finish();
        return 0;
    end else begin
        return 1;
    end
  endfunction

  function zuspec_message(string msg);
    $display("ZuspecSv: %0s", msg);
  endfunction
  export "DPI-C" function zuspec_message;

  function zuspec_error(string msg);
    $display("ZuspecSv ERROR: %0s", msg);
  endfunction
  export "DPI-C" function zuspec_error;

  function zuspec_fatal(string msg);
    $display("ZuspecSv FATAL: %0s", msg);
    $finish;
  endfunction
  export "DPI-C" function zuspec_fatal;

  /******************************************************************
   * DPI functions
   ******************************************************************/

  import "DPI-C" function chandle zuspec_Actor_new(
    string              randstate,
    string              comp_t,
    string              action_t,
    longint unsigned    backend_h);

  import "DPI-C" function longint unsigned zuspec_EvalBackendProxy_new();

  import "DPI-C" function int unsigned zuspec_init(
    string pss_files,
    int    load
  );

  import "DPI-C" function int unsigned zuspec_ValRefList_size(
    chandle list_h
  );
  import "DPI-C" function chandle zuspec_ValRefList_at(
    chandle list_h,
    int     idx
  );

  function void zuspec_EvalBackendProxy_callFuncReq(
    longint unsigned    proxy_h,
    chandle             thread_h,
    int                 func_id,
    chandle             params_h);
    Backend     backend = Backend::proxy2backend_m[proxy_h];
    EvalThread  thread = new(thread_h);
    ValRef params[];
    
    params = new[zuspec_ValRefList_size(params_h)];

    foreach (params[i]) begin
        params[i] = new(zuspec_ValRefList_at(params_h, i));
    end

    backend.callFuncReq(thread, func_id, params);
  endfunction
  export "DPI-C" function zuspec_EvalBackendProxy_callFuncReq;

  function void zuspec_EvalBackendProxy_emitMessage(
    longint unsigned    proxy_h,
    string              msg);
    Backend     backend = Backend::proxy2backend_m[proxy_h];
    backend.emitMessage(msg);
  endfunction
  export "DPI-C" function zuspec_EvalBackendProxy_emitMessage;

endpackage
