# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ROOTCuts)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/Plotting.cc)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/Yield_Adder.cc)
list(REMOVE_ITEM headers ${PROJECT_SOURCE_DIR}/include/ROOTEffs.hh)

file(GLOB sources2 ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers2 ${PROJECT_SOURCE_DIR}/include/*.hh)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/ROOTCuts.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/alpha_T.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/BiasedDeltaPhi.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/Christmas.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/CutsFunction.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/CutsFunctionBkg.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/Jet2Plus1Finder.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/Jet2Plus1MbbFinder.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/JetDoubleMbbPairFinder.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/JetDoublePairFinder.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/JetPairFinder.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/TerminalPlot.cc)
list(REMOVE_ITEM sources2 ${PROJECT_SOURCE_DIR}/src/YieldGetter.cc)
list(REMOVE_ITEM headers2 ${PROJECT_SOURCE_DIR}/include/TerminalPlot.hh)
list(REMOVE_ITEM headers2 ${PROJECT_SOURCE_DIR}/include/Ulrich_cuts.hh)
list(REMOVE_ITEM headers2 ${PROJECT_SOURCE_DIR}/include/BiasedDeltaPhi.hh)


set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
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
if(DEFINED ROOT_USE_FILE)    #---If ROOT was built with the classic configure/make---
    include(${ROOT_USE_FILE})
endif(DEFINED ROOT_USE_FILE)

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${DelphesExternals_INCLUDE_DIR} ${DELPHES})
add_definitions(${ROOT_CXX_FLAGS} -std=c++11)
link_directories(${DELPHES})

#---Create  a main program using the library
add_executable(ROOTEffs ROOTEffs.cc ${sources2} ${headers2})
target_link_libraries(ROOTEffs ${ROOT_LIBRARIES})

add_executable(ROOTCuts ROOTcuts.cc ${sources} ${headers})
target_link_libraries(ROOTCuts Delphes ${ROOT_LIBRARIES})


install(TARGETS ROOTCuts DESTINATION ${PROJECT_SOURCE_DIR})
install(TARGETS ROOTEffs DESTINATION ${PROJECT_SOURCE_DIR})
