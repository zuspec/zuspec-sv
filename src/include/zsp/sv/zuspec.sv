/*
 * zuspec.sv
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */

// `define ZUSPEC_DEBUG_EN

`ifdef ZUSPEC_DEBUG_EN
`define ZUSPEC_DEBUG(msg) $display msg
`else
`define ZUSPEC_DEBUG(msg)
`endif

`define ZUSPEC_FATAL(msg) \
    $display msg; \
    $finish

package zuspec;

  typedef class EvalThread;
  typedef class ValRef;
  typedef class Actor;

  class NullBase;
    // empty class to use as base type
  endclass

  class MethodBridge;
    Actor           m_actor;

    virtual function void init(Actor actor);
        m_actor = actor;
    endfunction

    virtual task invokeFuncTarget(
        EvalThread      thread,
        int             func_id,
        ValRef          params[]);
        `ZUSPEC_FATAL(("FATAL: zuspec::Backend::invokeFuncTarget not implemented"));
    endtask

    virtual function void invokeFuncSolve(
        EvalThread      thread,
        int             func_id,
        ValRef          params[]);
        `ZUSPEC_FATAL(("FATAL: zuspec::Backend::invokeFuncSolve not implemented"));
    endfunction

  endclass

  class Actor;
    static Actor proxy2actor_m[longint unsigned];
    chandle              m_hndl;
    string               m_name;
    MethodBridge             m_method_if;
    int unsigned         m_pending_tasks = 0;
    semaphore            m_task_sem = new();

    function new(
        string          comp_t,
        string          action_t,
        MethodBridge    method_if,        
        string          name="");
        process p = process::self();
        string randstate = p.get_randstate();
        longint unsigned backend_h;

        m_name = name;
        m_method_if = method_if;
        backend_h = zuspec_EvalBackendProxy_new();
        proxy2actor_m[backend_h] = this;

        m_hndl = zuspec_Actor_new(
            randstate,
            comp_t, 
            action_t,
            backend_h);

        if (m_hndl == null) begin
            `ZUSPEC_FATAL(("FATAL: zuspec_Actor_new returned null"));
        end

        m_method_if.init(this);

    endfunction

    task run();
        int ret = 0;

        // TODO:
        do begin
            ret = zuspec_Actor_eval(m_hndl);

            `ZUSPEC_DEBUG(("ret=%0d pending_tasks=%0d", ret, m_pending_tasks));
            if (m_pending_tasks > 0) begin
                `ZUSPEC_DEBUG(("--> wait_sem"));
                m_task_sem.get();
                `ZUSPEC_DEBUG(("<-- wait_sem"));
            end else if (ret) begin
                `ZUSPEC_FATAL(("Zuspec FATAL: evaluation stalled"));
                break;
            end
        end while (ret == 1);
    endtask

    virtual function void callFuncReq(
        EvalThread      thread,
        chandle         func_t,
        bit             is_target,
        ValRef          params[]);
        int func_id = zuspec_Actor_getFunctionId(m_hndl, func_t);

        if (func_id == -1) begin
            `ZUSPEC_FATAL(("Zuspec FATAL: No mapping for function %0s",
                zuspec_DataTypeFunction_name(func_t)));
        end else begin
            if (is_target) begin
                m_pending_tasks += 1;
                fork
                    begin
                        automatic int l_func_id = func_id;
                        m_method_if.invokeFuncTarget(thread, l_func_id, params);
                        m_pending_tasks -= 1;
                        m_task_sem.put(1);
                    end
                join_none
            end else begin
                m_method_if.invokeFuncSolve(thread, func_id, params);
            end
        end
    endfunction

    virtual function void emitMessage(string msg);
        $display("Zuspec %0s: %0s", m_name, msg);
    endfunction

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

    function void setVoidResult();
        zuspec_EvalThread_setVoidResult(m_hndl);
    endfunction

    function void setIntResult(
        longint value,
        bit     is_signed,
        int     width);
        zuspec_EvalThread_setIntResult(m_hndl, value, int'(is_signed), width);
    endfunction

  endclass

  bit _init = pkg_init();
  function bit pkg_init();
    automatic string pss_files;
    automatic int load = 0;
    automatic int debug = 0;
    automatic process p = process::self();

    `ZUSPEC_DEBUG(("randstate: %0s", p.get_randstate()));
    if ($value$plusargs("zuspec.pssfiles=%s", pss_files)) begin
        `ZUSPEC_DEBUG(("File: %0s", pss_files));
    end
    if ($value$plusargs("zuspec.load=%d", load)) begin
    end
    if ($value$plusargs("zuspec.debug=%d", debug)) begin
    end

    if (zuspec_init(pss_files, load, debug) != 1) begin
        `ZUSPEC_FATAL(("FATAL: Failed to initialize Zuspec package"));
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
  import "DPI-C" function int zuspec_Actor_eval(
    chandle             actor_h);
  import "DPI-C" function int unsigned zuspec_Actor_registerFunctionId(
    chandle             actor_h,
    string              name,
    int                 id);
  import "DPI-C" function int zuspec_Actor_getFunctionId(
    chandle             actor_h,
    chandle             func_h);

  import "DPI-C" function string zuspec_DataTypeFunction_name(
    chandle             func_h);

  import "DPI-C" function longint unsigned zuspec_EvalBackendProxy_new();


  import "DPI-C" function int unsigned zuspec_init(
    string pss_files,
    int    load,
    int    debug_en
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
    chandle             func_t,
    int unsigned        is_target,
    chandle             params_h);
    automatic Actor       actor = Actor::proxy2actor_m[proxy_h];
    automatic EvalThread  thread = new(thread_h);
    automatic ValRef params[];
    
    params = new[zuspec_ValRefList_size(params_h)];

    foreach (params[i]) begin
        params[i] = new(zuspec_ValRefList_at(params_h, i));
    end

    actor.callFuncReq(thread, func_t, is_target, params);
  endfunction
  export "DPI-C" function zuspec_EvalBackendProxy_callFuncReq;

  function void zuspec_EvalBackendProxy_emitMessage(
    longint unsigned    proxy_h,
    string              msg);
    automatic Actor     actor = Actor::proxy2actor_m[proxy_h];
    actor.emitMessage(msg);
  endfunction
  export "DPI-C" function zuspec_EvalBackendProxy_emitMessage;

  import "DPI-C" function void zuspec_EvalThread_setVoidResult(
    chandle             thread_h
  );

  import "DPI-C" function void zuspec_EvalThread_setIntResult(
    chandle             thread_h,
    longint             value,
    int                 is_signed,
    int                 width);

endpackage

`undef DEBUG