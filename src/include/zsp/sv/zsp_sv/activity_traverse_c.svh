/*
 * activity_traverse_c.svh
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
// Traversal constraints are added via aspects (?)

typedef class activity_traverse_base_c;
typedef class action_constraint_base_c;
typedef class executor_group_base_c;

class activity_traverse_c #(type Ta=action_c) extends activity_traverse_base_c;

    function new(actor_base_c actor, component_c parent_comp, Ta action=null);
        super.new(actor, parent_comp);
    endfunction

    virtual function action_type_c get_action_type();
        action_type_c action_t = Ta ::get_type();
        `ZSP_DEBUG("activity_traverse", ("get_action_type: %0s", action_t.name));
        return action_t;
    endfunction

    virtual task run_body(executor_base_c exec_b);
        action.body(exec_b);
    endtask

endclass

