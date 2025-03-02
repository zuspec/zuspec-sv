/*
 * reg_field_c.svh
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
class reg_field_c;
    string          name;
    bit[63:0]       offset;

    function new(string name);
        this.name = name;
    endfunction

    virtual function void map(
        executor_base_c exec_b,
        reg_group_c     group);
        offset = group.get_offset_of_instance(exec_b, name);
    endfunction

endclass
