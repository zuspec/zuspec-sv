/*
 * pool_c.svh
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
typedef class component_c;
typedef class component_ctor_ctxt;

class pool_base_c;
    string          name;
    component_c     parent;
    obj_type_c      obj_t;

    // function new(
    //     string              name,
    //     component_ctor_ctxt ctxt,
    //     component_c         parent,
    //     obj_type_c          obj_t);
    //     this.name = name;
    //     this.parent = parent;
    //     this.obj_t = obj_t;

    //     // Add ourselves to the parent's pool list
    //     parent.add_pool(this);
    // endfunction

    virtual function obj_type_c get_item_type();
        return this.obj_t;
    endfunction

endclass

// class pool_c extends pool_base_c;
//     int             id;

//     function new(string name, component_ctor_ctxt ctxt, component_c parent);
//         super.new(name, ctxt, parent, null);
//     endfunction

// endclass

