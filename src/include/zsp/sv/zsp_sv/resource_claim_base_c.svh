/*
 * resource_claim_base_c.svh
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
typedef class action_c;
typedef class resource_c;

// Need a base class for all refs (?)

class resource_claim_base_c;
    int       ref_id; // Index within action type. Used to discover pool
    bit       lock;

    function new(string name, action_c parent, bit lock);
        this.lock = lock;
        ref_id = parent.add_resource_claim(this);
    endfunction

    virtual function void set(resource_c rsrc);
    endfunction

    virtual function obj_type_c get_type();
        $display("Fatal: get_type not implemented");
        return null;
    endfunction

endclass