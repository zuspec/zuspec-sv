/*
 * reg_group_c.svh
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

typedef class reg_group_field_arr_c;
class reg_group_c;
    reg_field_c     fields[$];

    function new();
    endfunction


    function void map_registers(executor_base_c exec_b);
        $display("map_registers");
        foreach (fields[i]) begin
            reg_group_field_arr_c arr;

            $display("fields[i]: %0s", fields[i].name);
            fields[i].map(exec_b, this);

            // if ($cast(arr, fields[i])) begin
            //     $display("%0s: array", fields[i].name);
            //     foreach (arr.offsets[x]) begin
            //         arr.offsets[x] = get_offset_of_instance_array(
            //             exec_b, fields[i].name, i);
            //         $display("[%0d]: 0x%08h", x, arr.offsets[x]);
            //     end
            // end else begin
            //     fields[i].offset = get_offset_of_instance(exec_b, fields[i].name);
            // end
            $display("reg: %0s %0d", fields[i].name, fields[i].offset);
        end
    endfunction

    virtual function bit[63:0] get_offset_of_instance(executor_base_c exec_b, string name);
        return {64{1'b1}};
    endfunction

    virtual function bit[63:0] get_offset_of_instance_array(executor_base_c exec_b, string name, int index);
        return {64{1'b1}};
    endfunction

endclass
