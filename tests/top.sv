
module top;
  import zuspec::*;
  import pss_api_pkg::*;

  interface class ApiIF;
	  pure virtual task doit();
  endclass

  // Generated base class with APIs defined
  class MyApiIF #(type BASE_T=NullBase) extends BASE_T implements ApiIF;
	  virtual task doit();
		  $display("Zupspec FATAL: doit not implemented");
		  $finish;
	  endtask
  endclass

  // Specific bridge class that knows how to invoke APIs
`ifdef UNDEFINED
  class MyMethodBridge #(type TARGET_T=MyApiIF) extends MethodBridge;
	  TARGET_T m_target;

	  function new(TARGET_T target=null);
		  super.new();
		  m_target = target;
	  endfunction

	  function void init(Actor actor);
            super.init(actor);
		  if (!zuspec_Actor_registerFunctionId(actor.m_hndl, "doit", 1)) begin
			  $display("Failed to register doit");
		  end
	  endfunction

	  virtual task invokeFuncTarget(
		  EvalThread		thread,
		  int			func_id,
		  ValRef		params[]);
	  	$display("Invoke function target");
		case (func_id) 
		1: begin
		  	m_target.doit();
            thread.setVoidResult();
		end
		default: begin
			$display("Zuspec FATAL: Unknown function with ID %0d", func_id);
			$finish;
		end
		endcase
	endtask
  endclass
  `endif

  class MyBaseClass;
	  function new(string name="");
	  endfunction
  endclass

  // User class that inherits from base API
  class MyClass extends PssBaseIF #(MyBaseClass);
    typedef MyClass this_t;
    Actor #(this_t) m_actor;
    
    function new(string name="");
      this_t executors[] = '{this};
      super.new(name);
      m_actor = new("pss_top", "pss_top::Entry", executors);
    endfunction

    virtual task doit();
      $display("task doit");
    endtask

    virtual task run();
      m_actor.run();
    endtask
  endclass

  initial begin
    MyClass c = new();
    $display("Created new actor");
    c.run();
  end

endmodule


