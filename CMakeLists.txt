cmake_minimum_required(VERSION 3.16)

# Set compilers to Linux native
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(B1)

# Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Geant4
find_package(Geant4 REQUIRED ui_all vis_all)

# ROOT
find_package(ROOT REQUIRED COMPONENTS Core RIO Hist Tree)

# TBB
find_package(TBB REQUIRED)

# Sources
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Executable
add_executable(main main.cc ${sources} ${headers})

# Include
target_include_directories(main PRIVATE
    include
    ${Geant4_INCLUDE_DIRS}
)


target_link_libraries(main PRIVATE
    Threads::Threads
    ${Geant4_LIBRARIES}
    ROOT::Core
    ROOT::RIO
    ROOT::Hist
    ROOT::Tree
    TBB::tbb
)