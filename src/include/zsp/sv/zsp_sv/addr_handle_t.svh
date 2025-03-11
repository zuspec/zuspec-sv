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
typedef class executor_base_c;

class addr_handle_t extends object_pool_base;
    executor_base_c     exec_b;
    storage_handle_s    base;
    bit[63:0]           offset;
    
    function new(
        executor_base_c     exec_b,
        storage_handle_s    base=null, 
        bit[63:0]           offset=0);
        this.exec_b = exec_b;
        this.base = base;
        this.offset = offset;
        if (base != null) begin
            base.inc();
        end
    endfunction

    virtual function void drop();
        if (exec_b != null) begin
            actor_base_c actor = exec_b.get_actor();
            $display("addr_handle_t::drop");
            actor.addr_handle_drop(this);
            if (base != null) begin
               base.dec();
            end
        end
    endfunction

    function addr_handle_t make_handle(bit[63:0] offset);
        addr_handle_t ret;
        if (this.base != null) begin
            ret = new(exec_b, this.base, this.offset+offset);
        end else begin
            ret = new(exec_b, null, this.offset+offset);
        end
        return ret;
    endfunction

    virtual function bit[63:0] addr_value();
        bit[63:0] ret = offset;
        if (this.base != null) begin
            ret += this.base.addr;
        end
        return ret;
    endfunction

    static function addr_handle_t create_default();
        return null;
    endfunction

endclass
