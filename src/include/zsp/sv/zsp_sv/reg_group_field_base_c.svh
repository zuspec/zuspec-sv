/*
 * reg_group_field_base_c.svh
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

class reg_group_field_base_c extends reg_field_c;

    function new(string name, component_c parent);
        super.new(name);
        if (parent != null) begin
            parent.reg_group_fields.push_back(this);
        end
    endfunction

    function void set_handle(addr_handle_t hndl);
        offset = hndl.addr_value();
    endfunction

    virtual function void do_init(executor_base_c exec_b);
    endfunction

    virtual function reg_group_c get_type();
        return null;
    endfunction

endclass
