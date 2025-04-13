/*
 * activity_ctxt_par_c.svh
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
//typedef class activity_visitor_c;
typedef class activity_ctxt_c;
typedef class activity_ctxt_par_branch_c;

class activity_ctxt_par_c extends activity_ctxt_c;
    activity_ctxt_par_branch_c branches[$];
    semaphore                  first_traversal_sem = new(0);
    semaphore                  branch_release_sem = new(0);
    semaphore                  branch_complete_sem = new(0);   

    virtual task run(activity_ctxt_c ctxt);
        // Start all branches
        // Wait for a traversal to check in on all branches
        //   or, for all branches to complete
        // Solve all first actions
        // for (int i=0; i<branches.size(); i++) begin
        //     automatic activity_ctxt_par_branch_c branch = branches[i];
        //     fork
        //         branch.run(this);
        //     join_none
        // end

        // Wait for the first traversal to check in
        first_traversal_sem.get(branches.size());

        // TODO: Collect up traversals that now must be co-solved
        // TODO: Setup and run context solver

        // Release the branches to continue
        branch_release_sem.put(branches.size());

        // Wait for all branches to complete
        branch_complete_sem.get(branches.size());

        // 
        // TODO: fork/join
        // TODO: create new context for each branch (?)
        // TODO: merge sub-context back to parent context
    endtask

    virtual task first_traversal(activity_ctxt_par_branch_c branch);
        // Notify that the branch has reached the first traversal
        first_traversal_sem.put(1);
        
        // Wait to synchronize with the other branches
        branch_release_sem.get(1);
    endtask

    virtual task branch_complete(activity_ctxt_par_branch_c branch);
        branch_complete_sem.put(1);
    endtask

    virtual task add_branch(activity_c branch);
        activity_ctxt_par_branch_c branch_ctxt;

        // Create a new context for the branch
        branch_ctxt = new(branch);
        branches.push_back(branch_ctxt);

        fork
            branch_ctxt.run(this);
        join_none
    endtask


//    virtual function void accept(activity_visitor_c v);
//        v.visit_parallel(this);
//    endfunction


endclass
