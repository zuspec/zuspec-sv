/*
 * action_c.svh
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
typedef class action_constraint_base_c;
typedef class resource_claim_base_c;

class action_c extends activity_c;
    resource_claim_base_c   rsrc_claims[$];
    `zsp_rand_arr action_constraint_base_c layered_constraints[$];

    function new();
        super.new(null, null);
    endfunction

    virtual task body(executor_base_c exec_b);
    endtask

    virtual function int add_resource_claim(resource_claim_base_c rsrc_claim);
        int ret = rsrc_claims.size(); // TODO: Must consider other refs as well
        rsrc_claims.push_back(rsrc_claim);
        return ret;
    endfunction

    virtual function executor_base_c get_executor();
        return null;
    endfunction

    virtual function void set_component(component_c comp);
        $display("FATAL: set_component not implemented");
        $finish;
    endfunction

    virtual function component_c get_component();
        $display("FATAL: get_component not implemented");
        $finish;
        return null;
    endfunction

    virtual function obj_type_c get_obj_comp_type();
        return null;
    endfunction

endclass
