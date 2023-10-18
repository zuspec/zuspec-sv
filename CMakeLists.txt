
cmake_minimum_required(VERSION 3.10)

project(zuspec-sv)
include (ExternalProject)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT PACKAGES_DIR)
  set(PACKAGES_DIR ${CMAKE_SOURCE_DIR}/packages)
  set(ENABLE_TESTING 1)
endif()

if (WIN32)
  set(PYTHON ${PACKAGES_DIR}/python/Scripts/python.exe)
else()
  set(PYTHON ${PACKAGES_DIR}/python/bin/python)
endif()

execute_process(COMMAND ${PYTHON} -m ivpm share cmake OUTPUT_VARIABLE IVPM_CMAKE_PATH)
list(APPEND CMAKE_MODULE_PATH ${IVPM_CMAKE_PATH})
message("IVPM_CMAKE_PATH: ${IVPM_CMAKE_PATH} ${CMAKE_MODULE_PATH}")

include(ivpm)

Ivpm_AddPythonExtProject(debug-mgr debug_mgr)
Ivpm_AddPythonExtProject(zuspec-parser zsp_parser)
Ivpm_AddPythonExtProject(zuspec-fe-parser zsp_fe_parser)
Ivpm_AddPythonExtProject(zuspec-arl-dm zsp_arl_dm)
Ivpm_AddPythonExtProject(zuspec-arl-eval zsp_arl_eval)
Ivpm_AddPythonExtProject(vsc-dm vsc_dm)
Ivpm_AddPythonExtProject(vsc-solvers vsc_solvers)

if(NOT WIN32)
  add_compile_options(-fPIC)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

add_subdirectory(src)

if (ENABLE_TESTING)
  # Testing is only enabled when libvsc is the top-level project
  enable_testing()

#  add_subdirectory(test)
endif()


