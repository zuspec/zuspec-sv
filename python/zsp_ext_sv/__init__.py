
from zuspec.cmd import CmdRegistry
from .generator.cmd_generate_sv import CmdGenerateSv

CmdRegistry.inst().addSubCommand("gen-sv-import-api", CmdGenerateSv.addGenerateSvCmd)