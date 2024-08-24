# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(Tools)

# 添加 lib/Tool 子目录中的 CMakeLists.txt 文件
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Tool)

# 使用 file(GLOB) 命令获取所有源文件和资源文件
file(GLOB CPP_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB RC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.rc")
list(APPEND CPP_LIST ${RC_LIST})

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Tool)

# 添加可执行文件目标 Tools，并指定其源文件
add_executable(Tools ${CPP_LIST})

# 设置链接器选项，将子系统设置为 Windows
set_target_properties(Tools PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

# 设置可执行文件的输出目录
set_target_properties(Tools PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# 遍历 CMAKE_CONFIGURATION_TYPES 列表中的每个配置类型
# 这里只需要 Debug 和 Release 配置
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
  if(config STREQUAL "Debug")
    set_target_properties(Tools PROPERTIES
      OUTPUT_NAME "Tools_Debug"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
    )
  elseif(config STREQUAL "Release")
    set_target_properties(Tools PROPERTIES
      OUTPUT_NAME "Tools"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${config}"
    )
  endif()
endforeach()

# 设置 CMake 配置类型
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# 检查编译器类型
if(MSVC)
  # MSVC-specific settings
  set_target_properties(Tools PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # GCC and Clang-specific settings
  set_target_properties(Tools PROPERTIES LINK_FLAGS "-Wl,--subsystem,windows")
endif()
