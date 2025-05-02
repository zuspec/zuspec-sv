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

typedef class action_handle_c;
typedef class action_constraint_c;
typedef class action_init_c;
typedef class actor_base_c;
typedef class activity_traverse_base_c;
typedef class action_constraint_base_c;
typedef class executor_group_base_c;

class activity_traverse_c #(type Ta=action_c) extends activity_traverse_base_c;
    action_handle_c #(Ta)           action_h;
    action_init_c #(Ta)             action_init;
    action_constraint_c #(Ta)       action_constraint;

    function new(
        action_handle_c #(Ta)       action_h=null,
        action_init_c #(Ta)         action_init=null,
        action_constraint_c #(Ta)   action_constraint=null);
        super.new();
        if (action_h == null) begin
            action_h = new("<anonymous>");
        end
        this.action_h = action_h;
        this.action_init = action_init;
        this.action_constraint = action_constraint;
    endfunction

    virtual function action_type_c get_action_type();
        action_type_c action_t = Ta ::get_type();
        `ZSP_DEBUG("activity_traverse", ("get_action_type: %0s", action_t.name));
        return action_t;
    endfunction

    virtual function void set_action(action_c action);
        $cast(action_h.action, action);
    endfunction
    
    virtual function action_c get_action();
        return action_h.action;
    endfunction

    virtual function actor_base_c actor();
        return null; // TODO:
    endfunction

    virtual task run(activity_ctxt_c ctxt, int id=0);
        int pre_constraint_len;
        ctxt.enter_traverse(this);
        if (get_action() == null) begin
            solve_action_context_c ctxt_solver = new(ctxt.comp);

            // This traversal wasn't initialized by a containing activity
            action_h.action = new();

            void'(ctxt_solver.add_traversal(this));

            void'(ctxt_solver.resolve());


            // TODO: Setup and run context solver
            // action_h.initialize(null); // TODO: selected component
//            `ZSP_FATAL(("activity_traverse_c::run: action is null"));
        end

        if (action_init != null) begin
            action_init.initialize(action_h.action_i());
        end

        // TODO: identify and add layered constraints from above (?)
        pre_constraint_len = action_h.traversal_constraints.size();
        if (action_constraint != null) begin
            action_h.traversal_constraints.push_back(action_constraint);
        end

        action_h.run(ctxt);

        while (action_h.traversal_constraints.size() > pre_constraint_len) begin
            action_h.traversal_constraints.pop_back();
        end

        ctxt.leave_traverse(this);
    endtask

    static task do_run(action_handle_c #(Ta) action_h, activity_ctxt_c ctxt);
        activity_traverse_c #(Ta) action_t = new(action_h);
        action_t.run(ctxt);
    endtask

    static function activity_traverse_c #(Ta) mk(
        action_handle_c #(Ta)       action_h=null,
        action_init_c #(Ta)         action_init=null,
        action_constraint_c #(Ta)   action_constraint=null);
        activity_traverse_c #(Ta) ret = new(action_h, action_init, action_constraint);
        return ret;
    endfunction

endclass

