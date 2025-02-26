cmake_minimum_required(VERSION 3.10)
project(Tools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 子 CMake.txt
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Tool)


# 查找所有源文件和头文件
file(GLOB_RECURSE SOURCE_FILES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/**")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE libTypical_libHEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/libTypical/*.h")
file(GLOB_RECURSE libTypical_libHEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/libTypical/**/*.h")
file(GLOB_RECURSE Json_libHEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/Json/*.h")

list(APPEND FILE_LIST ${SOURCE_FILES_LIST} ${HEADER_FILES} ${RC_FILES} ${libTypical_libHEADER_FILES} ${Json_libHEADER_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libTypical)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Json)

add_executable(Tools ${FILE_LIST})

# 链接的库
# 使用 generator expressions 设置链接库
target_link_libraries(Tools
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/lib/libTypical/Debug_Win/libTypical.lib>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/lib/libTypical/Release_Win/libTypical.lib>
    
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/lib/Json/Debug/jsoncpp_static.lib>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/lib/Json/Release/jsoncpp_static.lib>
)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/bin)

set_target_properties(Tools PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
  LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

# 设置不同配置的编译定义
if (MSVC)
  target_compile_definitions(Tools
    PRIVATE
    _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
    _CRT_SECURE_NO_WARNINGS
  )
  # 对于所有配置设置 UTF-8 编码
  target_compile_options(Tools
    PRIVATE
    $<$<CONFIG:Debug>:/utf-8>
    $<$<CONFIG:Release>:/utf-8>
  )
  target_compile_definitions(Tools
    PRIVATE
    $<$<CONFIG:Debug>:_DEBUG;_WINDOWS;WIN32APP>
    $<$<CONFIG:Release>:NDEBUG;_WINDOWS;WIN32APP>
  )
endif()

# 分组源文件和头文件
source_group("HeaderFiles" FILES ${HEADER_FILES})
source_group("SourceFiles" FILES ${SOURCE_FILES_LIST})
source_group("Rc Files" FILES ${RC_FILES})
source_group("libTypical_libHeaderFiles" FILES ${libTypical_libHEADER_FILES})
source_group("Json_libHeaderFiles" FILES ${Json_libHEADER_FILES})