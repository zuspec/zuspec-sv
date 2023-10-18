
file(GLOB zsp_arl_eval_SRC
  "*.h"
  "*.cpp"
  )

add_library(zsp-sv SHARED ${zsp_arl_eval_SRC})

target_include_directories(zsp-sv PUBLIC
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${debug_mgr_INCDIR}
    ${zsp_arl_dm_INCDIR}
    ${zsp_arl_eval_INCDIR}
    ${zsp_fe_parser_INCDIR}
    ${zsp_parser_INCDIR}
    ${vsc_dm_INCDIR}
    ${vsc_solvers_INCDIR}
    )

target_link_directories(zsp-sv PRIVATE
    ${CMAKE_BINARY_DIR}/lib
    ${CMAKE_BINARY_DIR}/lib64
    ${zsp_arl_eval_LIBDIR}
    ${zsp_fe_parser_LIBDIR}
    ${zsp_arl_dm_LIBDIR}
    ${vsc_dm_LIBDIR}
    ${vsc_solvers_LIBDIR}
    ${zsp_parser_LIBDIR}
    ${debug_mgr_LIBDIR}
    )
target_link_libraries(zsp-sv
    zsp-arl-eval
    zsp-parser
    zsp-fe-parser
    vsc-solvers
    zsp-arl-dm
    ast
    vsc-dm
    debug-mgr)

#add_dependencies(zsp-sv 
#    LibVSC-DM 
#    LibVSC-Solvers)

install(TARGETS zsp-sv
    DESTINATION lib
    EXPORT zsp-sv-targets)

