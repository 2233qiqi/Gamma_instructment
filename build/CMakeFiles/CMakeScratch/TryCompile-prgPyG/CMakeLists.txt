cmake_minimum_required(VERSION 3.28.3.0)
set(CMAKE_MODULE_PATH "/usr/lib/x86_64-linux-gnu/cmake/vtk-9.1/patches/99;/usr/lib/x86_64-linux-gnu/cmake/vtk-9.1;/home/qiqi/ROOT/root/cmake/modules")
cmake_policy(SET CMP0126 OLD)
cmake_policy(SET CMP0128 OLD)
project(CMAKE_TRY_COMPILE C)
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILE_DEFINITIONS}")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXE_LINKER_FLAGS}")
include_directories(${INCLUDE_DIRECTORIES})
set(CMAKE_SUPPRESS_REGENERATION 1)
link_directories(${LINK_DIRECTORIES})

include("${CMAKE_CURRENT_LIST_DIR}//cmTC_11e2bTargets.cmake")

cmake_policy(SET CMP0065 NEW)
cmake_policy(SET CMP0083 OLD)
cmake_policy(SET CMP0155 OLD)
include("${CMAKE_ROOT}/Modules/Internal/HeaderpadWorkaround.cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/qiqi/geant4/Gamma_instructment/build/CMakeFiles/CMakeScratch/TryCompile-prgPyG")
add_executable(cmTC_11e2b)
target_sources(cmTC_11e2b PRIVATE
  "/home/qiqi/geant4/Gamma_instructment/build/CMakeFiles/CMakeScratch/TryCompile-prgPyG/test_mpi.c"
)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmTC_11e2b_loc"
     CONTENT $<TARGET_FILE:cmTC_11e2b>)
target_link_libraries(cmTC_11e2b  "MPI::MPI_C" )
