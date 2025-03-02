/*
 * actor_t_c.svh
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

/**
 * An actor is a special type of component that autonomously 
 * runs behavior
 */
class actor_t_c #(type Tc=component_c, type Ta=int) extends Tc;
    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        super.new(name, ctxt, parent);
    endfunction

    task run();
//        Ta action = new();
//        activity_c #(Ta) activity = action.get_activity();

    endtask

    virtual function void start(executor_base_c exec_b);
        fork
            run_activity();
        join_none
    endfunction

endclass
