set(FFMPEG_CORE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CXX_FLAGS
    -g
    -Wall
    -O3
    -std=c++11
    )

# 定义源码目录列表
set(MODULES_SOURCE_DIR
    ${FFMPEG_CORE_SOURCE_DIR}
    ${FFMPEG_CORE_SOURCE_DIR}/Other
    ${FFMPEG_CORE_SOURCE_DIR}/Log
)

# 递归遍历收集所有源文件
set(ALL_SOURCE_CODE "")
foreach(file_path ${MODULES_SOURCE_DIR})
    file(GLOB_RECURSE file_source 
            ${file_path}/*.cc ${file_path}/*.h
            ${file_path}/*.mm ${file_path}/*.cpp
    )
    list(APPEND ALL_SOURCE_CODE ${file_source})
endforeach()

# 创建静态库
add_library(ffmpeg_util STATIC ${ALL_SOURCE_CODE})

# RUNTIME_OUTPUT_DIRECTORY：控制可执行文件​（.exe/无后缀）的输出路径
# ARCHIVE_OUTPUT_DIRECTORY：控制静态库​（.a/.lib）的输出路径
# LIBRARY_OUTPUT_DIRECTORY：控制动态库​（.so/.dll）的输出路径
set_target_properties(ffmpeg_util PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib"
)

target_link_libraries(ffmpeg_util PRIVATE spdlog::spdlog ${FFMPEG_LIBRARIES})
target_link_directories(ffmpeg_util PRIVATE ${FFMPEG_LIBRARY_DIRS})

target_include_directories(ffmpeg_util PUBLIC ${FFMPEG_INCLUDE_DIRS} ${SPDLOG_INCLUDES})
target_include_directories(ffmpeg_util PUBLIC ${MODULES_SOURCE_DIR})