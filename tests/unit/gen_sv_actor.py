
import os
import logging
from dv_flow.mgr import TaskDataResult, FileSet, TaskMarker, TaskMarkerLoc
from pydantic import BaseModel

_log = logging.getLogger("GenSvActor")

async def GenSvActor(runner, input):
    import zsp_fe_parser.core as zsp_fe
    import zsp_arl_dm.core as zsp_arl
    import zsp_parser.core as zspp
    import zsp_sv.core as zsp_sv
    _log.debug("--> GenSvActor")

    changed = True
    markers = []
    status = 0

    files = []
    for fs in input.inputs:
        files.extend([os.path.join(fs.basedir, f) for f in fs.files])


    if changed:
        factory = zsp_fe.Factory.inst()
        if input.params.debug:
            factory.getDebugMgr().enable(True)
#        if logging.root.level >= logging.DEBUG:
#            factory.getDebugMgr().enable(True)

        arl_f = zsp_arl.Factory.inst()
        arl_ctxt = arl_f.mkContext()

        zsp_f = zspp.Factory.inst()
        marker_c = zsp_f.mkMarkerCollector()
        ast_builder = zsp_f.mkAstBuilder(marker_c)
        ast_linker = zsp_f.mkAstLinker()
        zsp_fe_f = zsp_fe.Factory.inst()

        load_stdlib = True

        ast_l = []
        if load_stdlib:
            core_lib = zsp_f.getAstFactory().mkGlobalScope(len(ast_l))
            zsp_f.loadStandardLibrary(ast_builder, core_lib)
            _log.debug("Children: %d" % core_lib.numChildren())
            ast_l.append(core_lib)

        for file in files:
            ast_root = zsp_f.getAstFactory().mkGlobalScope(len(ast_l))
            with open(file, "r") as fp:
                ast_builder.build(ast_root, fp)
            ast_l.append(ast_root)

        for m in marker_c.markers():
            # TODO: handle severity
            markers.append(TaskMarker(msg=m.msg()))
            print("Parse Marker: %s" % m.msg())
        if marker_c.hasSeverity(zspp.MarkerSeverityE.Error):
            status |= 1

        if status == 0:
            linked_root = ast_linker.link(marker_c, ast_l)
            for m in marker_c.markers():
                # TODO: handle severity
                markers.append(TaskMarker(msg=m.msg()))
            if marker_c.hasSeverity(zspp.MarkerSeverityE.Error):
                status |= 1

        if status == 0:
            ast2arl_builder = zsp_fe_f.mkAst2ArlBuilder()
            ast2arl_ctxt = zsp_fe_f.mkAst2ArlContext(
                arl_ctxt,
                linked_root,
                marker_c)
        
            ast2arl_builder.build(linked_root, ast2arl_ctxt)

            pss_top = arl_ctxt.findDataTypeStruct(input.params.component)
            if pss_top is None:
                markers.append(TaskMarker(
                    severity="error",
                    msg="pss_top \"%s\" could not be located (available: %s)" % (
                        input.params.component,
                        ",".join([t.name() for t in arl_ctxt.getDataTypeStructs()]))))
                status = 1

        if status == 0:
            pss_top_Entry = arl_ctxt.findDataTypeStruct(input.params.action)
            if pss_top_Entry is None:
                markers.append(TaskMarker(
                    severity="error",
                    msg="root action %s could not be found" % input.params.action))
                status = 1

        if status == 0: 
            zsp_sv_f = zsp_sv.Factory.inst()

            zsp_sv_f.prepContextExec(arl_ctxt)

            try:
                outfile = os.path.join(input.rundir, input.params.filename)
                fp = open(outfile, "w")
            except Exception as e:
                markers.append(TaskMarker(
                    severity="error",
                    msg="failed to open file %s for writing" % outfile))
                status = 1
        
        if status == 0:
            generator = zsp_sv_f.mkGenerateTypesPkg(
                arl_ctxt,
                fp)
            generator.generate()

            generator = zsp_sv_f.mkGenerateActorPkgPrv(
                arl_ctxt,
                pss_top,
                pss_top_Entry,
                fp)
            generator.generate()

            generator = zsp_sv_f.mkGenerateActorPkg(
                arl_ctxt,
                pss_top,
                pss_top_Entry,
                fp)
            generator.generate()
            fp.close()

    fs = FileSet(
        filetype="systemVerilogSource",
        src=input.name,
        basedir=input.rundir,
        files=[input.params.filename]
    )

    _log.debug("<-- GenSvActor")

    return TaskDataResult(
        memento=None,
        changed=changed,
        output=[fs],
        status=status
    )