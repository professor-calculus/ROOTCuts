# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ROOTCuts)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{DELPHES_DIR})

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
#find_package(delphes REQUIRED)

set(DelphesExternals_INCLUDE_DIR ${DELPHES}/external)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DelphesExternals_INCLUDE_DIR} ${DELPHES})
add_definitions(${ROOT_CXX_FLAGS} -std=c++11)
link_directories(${DELPHES})

#---Create  a main program using the library
add_executable(ROOTCuts ROOTcuts.cc ${sources} ${headers})
target_link_libraries(ROOTCuts Delphes ${ROOT_LIBRARIES})

install(TARGETS ROOTCuts DESTINATION ${PROJECT_SOURCE_DIR})
