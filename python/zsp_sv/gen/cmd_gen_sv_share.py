#****************************************************************************
#* cmd_gen_sv_actor.py
#*
#* Copyright 2023 Matthew Ballance and Contributors
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
import os

class CmdGenSvShare(object):

    def __call__(self, args):
        zsp_sv_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

        if os.path.isdir(os.path.join(zsp_sv_dir, "share")):
            print(os.path.join(zsp_sv_dir, "share"))
        else:
            # In a source-level environment
            zsp_sv_dir = os.path.abspath(os.path.join(zsp_sv_dir, "../.."))
            print(os.path.join(zsp_sv_dir, "src"))

    @staticmethod
    def register(cmd, subparsers):
        parser = subparsers.add_parser("synth.sv.share",
                        help="Print the path to the zuspec-sv 'share' directory")
        parser.set_defaults(func=CmdGenSvShare())
