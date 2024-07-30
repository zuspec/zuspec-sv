
#add_subdirectory(dpi)

file(GLOB zsp_sv_SRC
  "*.h"
  "*.cpp"
  "gen/*.h"
  "gen/*.cpp"
  "gen/exec/*.h"
  "gen/exec/*.cpp"
  )

add_library(zsp-sv SHARED ${zsp_sv_SRC})

target_include_directories(zsp-sv PUBLIC
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${debug_mgr_INCDIR}
    ${pyapi_compat_if_INCDIR}
    ${zsp_arl_dm_INCDIR}
    ${zsp_arl_eval_INCDIR}
    ${zsp_fe_parser_INCDIR}
    ${zsp_parser_INCDIR}
    ${vsc_dm_INCDIR}
    ${vsc_solvers_INCDIR}
    )

install(TARGETS zsp-sv
    DESTINATION lib
    EXPORT zsp-sv-targets)

