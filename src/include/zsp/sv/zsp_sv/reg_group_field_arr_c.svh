/*
 * reg_group_field_arr_c.svh
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


class reg_group_field_arr_c #(type group_t=reg_group_c) extends reg_group_field_base_c;
    group_t     group;
    bit[63:0]   offsets[];

    function new(string name, int sz, group_t group);
        super.new(name, null);
        offsets = new[sz];
        this.group = group;
    endfunction

    virtual function reg_group_c get_type();
        return group;
    endfunction

    virtual function void map(
        executor_base_c exec_b,
        reg_group_c     group);
        foreach (offsets[i]) begin
            offsets[i] = group.get_offset_of_instance_array(exec_b, name, i);
        end
    endfunction

endclass