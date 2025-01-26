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

typedef class action_constraint_base_c;

class activity_traverse_c #(type Ta) extends activity_c;
    rand Ta                         action;
`ifdef VERILATOR
    // As of 5.029, Verilator does not support random queues
    action_constraint_base_c   constraints[$];
`else
    rand action_constraint_base_c   constraints[$];
`endif

    // Core constraints:
    // - component id
    // - pool id

    function new(actor_c actor, component_c parent_comp, Ta action=null);
        super.new(actor, parent_comp);
        if (action == null) begin
            action = new();
        end
        this.action = action;
    endfunction

    function void add_constraint(action_constraint_base_c c);
        constraints.push_back(c);
    endfunction

    task run();
        executor_base exec_b = actor.get_default_executor();
        `ZSP_DEBUG_ENTER("activity_traverse_c", ("run"));

        action.init(actor, parent_comp);

        if (this.action.get_component() == null) begin
            // The context didn't assign component. Need to 
            // perform the context solving here...
            solve_action_context_c ctxt_solver = new(parent_comp);
            ctxt_solver.add_action(action);
            void'(ctxt_solver.resolve());
        end

        action.pre_solve();


        // Randomize action. Any traversals with an inline `with` 
        // get their own activity specialization
        // TODO: Handle errors
        if (!do_randomize()) begin
            $display("FATAL: activity_traverse_c::run failed to randomize");
            $finish;
        end

        // TODO: connect up the selected component
        action.post_solve(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].enter_traverse(action);
        end

        run_body(exec_b);

        foreach (actor.listeners[i]) begin
            actor.listeners[i].leave_traverse(action);
        end

        // begin
        //     Tact act = new(actor);
        //     act.run();
        // end

        `ZSP_DEBUG_LEAVE("activity_traverse_c", ("run"));
    endtask

    virtual task run_body(executor_base exec_b);
        action.body(exec_b);
    endtask

    function bit do_randomize();
        return action.randomize();
    endfunction

endclass

