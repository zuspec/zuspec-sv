/*
 * action_constraint_c.svh
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
class action_constraint_base_c;
    action_c        action;
endclass

class action_constraint_c #(type Ta=action_c) extends action_constraint_base_c;
    rand Ta         __self;

    function void pre_randomize();
        if (!$cast(__self, action)) begin
            `ZSP_FATAL(("Failed to cast action to %0s", Ta::get_type().name));
        end
    endfunction

endclass

// Define class between target <action> and parent <this>

