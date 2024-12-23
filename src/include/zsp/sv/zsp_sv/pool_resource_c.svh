/*
 * pool_resource_c.svh
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
typedef class resource_c;

class pool_resource_base_c extends pool_c;
    int         size;
    resource_c  resources[];

    function new(string name, component_c parent, int size);
        super.new(name, parent);
        this.size = size;
    endfunction

    // TODO: need to have pool participate in build process


endclass

class pool_resource_c #(type Tr=resource_c) extends pool_resource_base_c;

    function new(string name, component_c parent, int size);
        super.new(name, parent, size);
    endfunction

    function void build();
        resources = new[size];
        for (int i=0; i<size; i++) begin
            resources[i] = new(i);
        end
    endfunction

endclass


