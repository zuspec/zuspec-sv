/*
 * component_ctor_ctxt_c.svh
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

 typedef class action_type_c;

class component_ctor_ctxt_c;
    actor_base_c    actor;
    executor_base_c executor_m[];
    component_c     comp_inst_s[$];

    function new(actor_base_c actor, int n_executor_types);
        this.actor = actor;
        executor_m = new[n_executor_types+1];
    endfunction

    function executor_base_c get_default_executor();
        return actor.get_default_executor();
    endfunction

    function void enter(component_c comp);
        `ZSP_DEBUG_ENTER("component_ctor_c", ("enter %0s", comp.name));
        // if (!comp_inst_s.size || comp_inst_s[comp_inst_s.size-1] != comp) begin
        //     obj_type_c comp_t = comp.get_obj_type();
        //     comp_inst_s.push_back(comp);
        //     for (int i=comp_inst_s.size-1; i>=0; i--) begin
        //         comp_inst_s[i].add_comp_inst(comp);
        //     end
        // end
        `ZSP_DEBUG_LEAVE("component_ctor_c", ("enter %0s", comp.name));
    endfunction

    function void leave(component_c comp);
        `ZSP_DEBUG_ENTER("component_ctor_c", ("leave %0s", comp.name));
        // if (comp_inst_s.size && comp_inst_s[comp_inst_s.size-1] == comp) begin
        //     void'(comp_inst_s.pop_back());
        // end
        `ZSP_DEBUG_LEAVE("component_ctor_c", ("leave %0s", comp.name));
    endfunction

endclass

// Temp
typedef component_ctor_ctxt_c component_ctor_ctxt;