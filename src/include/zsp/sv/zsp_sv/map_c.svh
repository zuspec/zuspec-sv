/*
 * map_c.svh
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
class map_c #(type Tk=int, type Tv=int);
    Tv          m_store[Tk];

    virtual function Tv __getitem__(Tk key);
        return m_store[key];
    endfunction

    virtual function void __setitem__(Tk key, Tv val);
        m_store[key] = val;
    endfunction


endclass