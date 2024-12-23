/*
 * channel_c.svh
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
class channel_c #(type Te=int, int DEPTH=1) extends component_c;
    mailbox #(Te)       m_imp;

    function new(string name, component_ctor_ctxt ctxt, component_c parent=null);
        super.new(name, ctxt, parent);
        m_imp = new(DEPTH);
    endfunction

    task put(Te t);
        m_imp.put(t);
    endtask

    function bit try_put(Te t);
        return m_imp.try_put(t);
    end

    task get(output Te t);
        m_imp.get(t);
    endtask

    function Te try_get();
        Te ret;
        if (try_get(ret)) begin
            return ret;
        end else begin
            return null;
        end
    endfunction

endclass