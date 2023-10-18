/**
 * ZuspecSvDpiImp.h
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
#pragma once

typedef void *chandle;
extern "C" void zuspec_message(const char *msg);
extern "C" void zuspec_error(const char *msg);
extern "C" void zuspec_fatal(const char *msg);

extern "C" void zuspec_EvalBackendProxy_emitMessage(
    chandle     proxy_h,
    const char *msg);
extern "C" void zuspec_EvalBackendProxy_callFuncReq(
    chandle             proxy_h,
    chandle             thread_h,
    int32_t             func_id,
    const chandle       params_h
);


