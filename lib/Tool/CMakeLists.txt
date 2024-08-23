cmake_minimum_required(VERSION 3.10)

project(libTools_Tool)

add_compile_definitions(_UNICODE)
add_compile_definitions(TOOLS_TOOL_EXPORTS)

file(GLOB SRC_Tool_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/Tool/*.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/Tool)

add_library(libTools_Tool SHARED ${SRC_Tool_LIST})

set_target_properties(libTools_Tool PROPERTIES
  #LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin
)
