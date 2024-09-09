cmake_minimum_required(VERSION 3.10)
project(Tools)

# 子 CMake.txt
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Tool)

# 查找所有源文件和头文件
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE libHEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/libTypical_Tool/*.h")
file(GLOB_RECURSE RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.rc")

list(APPEND FILE_LIST ${SOURCE_FILES} ${HEADER_FILES} ${RC_FILES} ${libHEADER_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libTypical_Tool)

add_executable(Tools ${FILE_LIST})

# 链接的库
# 使用 generator expressions 设置链接库
target_link_libraries(Tools
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/lib/libTypical_Tool/Debug_Win/libTypical_Tool.lib>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/lib/libTypical_Tool/Release_Win/libTypical_Tool.lib>
)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin)

set_target_properties(Tools PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# 设置不同配置的编译定义
target_compile_definitions(Tools
  PRIVATE
  $<$<CONFIG:Debug>:_DEBUG;_WINDOWS>
  $<$<CONFIG:Release>:NDEBUG;_WINDOWS>
)

if(MSVC)
  set_target_properties(Tools PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set_target_properties(Tools PROPERTIES LINK_FLAGS "-Wl,--subsystem,windows")
endif()

# 分组源文件和头文件
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Rc Files" FILES ${RC_FILES})
source_group("lib Header Files" FILES ${libHEADER_FILES})