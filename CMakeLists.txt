cmake_minimum_required(VERSION 3.12)

project(ia-tp3)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dist)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

set(CMAKE_CXX_STANDARD 23)

include("utils/files_and_folders.cmake")
include(vendors/imgui.cmake)

add_subdirectory(vendors/toto-engine)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

add_executable(ia-tp3 ${SOURCES})

target_include_directories(ia-tp3 PUBLIC ${INC_DIR})

target_link_libraries(ia-tp3 PRIVATE toto-engine imgui)

Cool__target_copy_folder(ia-tp3 resources)
