/*
 * activity_c.svh
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
typedef class actor_base_c;

class activity_c extends object;
    actor_base_c        actor;
    component_c         parent_comp;

    function new(actor_base_c actor, component_c parent_comp);
        this.actor = actor;
        this.parent_comp = parent_comp;
    endfunction

    function void init(actor_base_c actor, component_c parent_comp);
        this.actor = actor;
        this.parent_comp = parent_comp;
    endfunction

    virtual task run();
        $display("FATAL: activity::run not implemented");
        $finish;
    endtask

endclass