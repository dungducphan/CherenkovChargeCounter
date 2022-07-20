cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(vuv)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.hh)

file(GLOB MACROS "*.mac")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${EXT_DATA} DESTINATION ${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(vuv sim.cc ${sources} ${headers})
target_link_libraries(vuv ${Geant4_LIBRARIES})
target_link_libraries(vuv ${ROOT_LIBRARIES})
