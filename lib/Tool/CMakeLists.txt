# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(libTools_Tool)

# 添加编译定义
add_compile_definitions(TOOLS_TOOL_EXPORTS)

# 获取所有源文件
file(GLOB SRC_Tool_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/Tool/*.cpp")

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include/Tool)

# 创建静态库目标
add_library(libTools_Tool STATIC ${SRC_Tool_LIST})

# 设置库的输出目录和不同配置的属性
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
  if(config STREQUAL "Debug_Win")
    set_target_properties(libTools_Tool PROPERTIES
      OUTPUT_NAME "libTools_Tool_Win"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${config}"
    )
  elseif(config STREQUAL "Release_Win")
    set_target_properties(libTools_Tool PROPERTIES
      OUTPUT_NAME "libTools_Tool_Win"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${config}"
    )
  elseif(config STREQUAL "Debug")
    set_target_properties(libTools_Tool PROPERTIES
      OUTPUT_NAME "libTools_Tool"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${config}"
    )
  elseif(config STREQUAL "Release")
    set_target_properties(libTools_Tool PROPERTIES
      OUTPUT_NAME "libTools_Tool"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${config}"
    )
  endif()
endforeach()
