from zuspec.cli import add_subcommand
from .gen.cmd_generate_sv import CmdGenerateSv
from .gen.cmd_gen_sv_actor import CmdGenSvActor

add_subcommand("synth.sv.actor", CmdGenSvActor.register)
