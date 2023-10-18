#****************************************************************************
#* __main__.py
#*
#* Copyright 2022 Matthew Ballance and Contributors
#*
#* Licensed under the Apache License, Version 2.0 (the "License"); you may 
#* not use this file except in compliance with the License.  
#* You may obtain a copy of the License at:
#*
#*   http://www.apache.org/licenses/LICENSE-2.0
#*
#* Unless required by applicable law or agreed to in writing, software 
#* distributed under the License is distributed on an "AS IS" BASIS, 
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
#* See the License for the specific language governing permissions and 
#* limitations under the License.
#*
#* Created on:
#*     Author: 
#*
#****************************************************************************
import argparse
import debug_mgr
import vsc_dm
import vsc_solvers
import zsp_parser
import zsp_fe_parser
import zsp_arl_dm
import zsp_arl_eval

def cmd_ldpath(args):
    pkgs = [debug_mgr, vsc_dm, vsc_solvers, zsp_parser, zsp_fe_parser, zsp_arl_dm, zsp_arl_eval]
    paths = set()
    for p in pkgs:
        for d in p.get_libdirs():
            paths.add(d)

    print(":".join(paths))

def cmd_ldflags(args):
    pkgs = [
        zsp_arl_eval,
        zsp_fe_parser, 
        zsp_arl_dm, 
        zsp_parser, 
        vsc_solvers, 
        vsc_dm,
        debug_mgr
    ]
    paths = []
    libs = []

    for p in pkgs:
        for d in p.get_libdirs():
            if d not in paths:
                paths.append(d)
        for l in p.get_libs():
            if l not in libs:
                libs.append(l)
    flags = []
    for p in paths:
        flags.append("-L%s" % p)
        flags.append("-Wl,-rpath=%s" % p)
    for l in libs:
        flags.append("-l%s" % l)
    
    print(" ".join(flags))

def getparser():
    parser = argparse.ArgumentParser()
    subparser = parser.add_subparsers()
    subparser.required = True
    ldpath = subparser.add_parser("ldpath")
    ldpath.set_defaults(func=cmd_ldpath)
    ldflags = subparser.add_parser("ldflags")
    ldflags.set_defaults(func=cmd_ldflags)

    return parser

def main():
    parser = getparser()
    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()


