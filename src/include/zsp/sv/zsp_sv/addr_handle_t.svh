/*
 * addr_handle_t.svh
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
typedef class object_ref_base_c;
typedef class storage_handle_c;

class addr_handle_t extends object_ref_c #(storage_handle_c);
    bit[63:0]           offset;

    function new(
        storage_handle_c    base=null,
        bit[63:0]           offset=0);
        super.new(base);
        this.offset = offset;
    endfunction

    function addr_handle_t make_handle(bit[63:0] offset);
        addr_handle_t ret;
        storage_handle_c obj;

        if (this.obj != null) begin
            obj = get();
        end

        ret = new(obj, this.offset+offset);
        return ret;
    endfunction

    virtual function bit[63:0] addr_value();
        bit[63:0] ret = offset;
        if (this.obj != null) begin
            ret += get().addr;
        end
        return ret;
    endfunction

    static function addr_handle_t create_default();
        addr_handle_t ret = new();
        return ret;
    endfunction

endclass
