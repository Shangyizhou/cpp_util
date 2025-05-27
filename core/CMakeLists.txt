set(TOOL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CXX_FLAGS
    -g
    -Wall
    -O3
    -std=c++11
    )

# 定义源码目录列表
set(MODULES_SOURCE_DIR
    ${TOOL_SOURCE_DIR}
    ${TOOL_SOURCE_DIR}/Media
    ${TOOL_SOURCE_DIR}/File
    ${TOOL_SOURCE_DIR}/Log
    ${TOOL_SOURCE_DIR}/Timer
    ${TOOL_SOURCE_DIR}/proto
    ${TOOL_SOURCE_DIR}/Foundation
    ${TOOL_SOURCE_DIR}/DB
    ${TOOL_SOURCE_DIR}/Strand
)

# 递归遍历收集所有源文件
set(TOOL_ALL_SOURCE_CODE "")
foreach(file_path ${MODULES_SOURCE_DIR})
    file(GLOB_RECURSE file_source 
            ${file_path}/*.cc ${file_path}/*.h
            ${file_path}/*.mm ${file_path}/*.cpp
    )
    list(APPEND TOOL_ALL_SOURCE_CODE ${file_source})
endforeach()

# 创建静态库
add_library(tool STATIC ${TOOL_ALL_SOURCE_CODE})

option(ENABLE_ASAN "Enable AddressSanitizer" ON)
if(ENABLE_ASAN)
    # 添加编译和链接选项
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
    message(STATUS "AddressSanitizer enabled")
endif()

# tool 使用 spdlog 依赖，但是依赖 tool 的不会依赖 spdlog
target_link_libraries(tool PRIVATE spdlog::spdlog protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite ${FFMPEG_LIBRARY_DIRS} ${FFMPEG_LIBRARIES})
target_link_libraries(tool PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(tool PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(tool PRIVATE miniz::miniz)
target_link_libraries(tool PRIVATE sqlite_orm::sqlite_orm)

target_link_libraries(tool
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

target_include_directories(tool PUBLIC 
    ${SPDLOG_INCLUDES} 
    ${PROTOBUF_INCLUDES} 
    ${GTEST_INCLUDE_DIRS} 
    ${MINIZ_INCLUDE_DIRS}
    ${FFMPEG_INCLUDE_DIRS} 
    ${SDL_INCLUDE_DIRS} 
    ${MODULES_SOURCE_DIR}
)

# PUBLIC 表示这些头文件路径会传播给任何链接 tool 的目标，这些目标自动包含 File/ 和 Log/ 目录的头文件
target_include_directories(tool PUBLIC ${MODULES_SOURCE_DIR}) 