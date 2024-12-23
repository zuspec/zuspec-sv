/*
 * list_c.svh
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
class list_c #(type T=int);
    rand T       store[$];

    function T __at__(int idx);
        return store[idx];
    endfunction

    function T __getitem__(int idx);
        return store[idx];
    endfunction

    function void __setitem__(int idx, T item);
        store[idx] = item;
    endfunction

    function int size();
        return store.size();
    endfunction

    function void clear();
        store = {};
    endfunction

    function void delete(int idx);
        store.delete(idx);
    endfunction

    function void insert(int idx, T elem);
        store.insert(idx, elem);
    endfunction

    function void push_back(T item);
        store.push_back(item);
    endfunction

endclass