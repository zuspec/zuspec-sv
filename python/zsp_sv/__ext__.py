from zuspec.cli import add_subcommand
from .gen.cmd_generate_sv import CmdGenerateSv
from .gen.cmd_gen_sv_actor import CmdGenSvActor
from .gen.cmd_gen_sv_share import CmdGenSvShare

add_subcommand("synth.sv.actor", CmdGenSvActor.register)
add_subcommand("synth.sv.share", CmdGenSvShare.register)
