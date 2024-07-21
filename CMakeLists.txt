cmake_minimum_required(VERSION 3.10)

project(Tools)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Tool)

add_compile_definitions(_UNICODE)

file(GLOB CPP_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB RC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.rc")

list(APPEND CPP_LIST ${RC_LIST})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Tool)

add_executable(Tools ${CPP_LIST})



target_link_libraries(Tools libTools_Tool)

set_target_properties(Tools PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

set_target_properties(Tools PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)