# 指定使用 CMake 的最低版本号
cmake_minimum_required(VERSION 3.0)

# 设置项目名称
project(SDK C CXX)

# 添加搜索路径
include_directories(
    ${PROJECT_SOURCE_DIR}/sdk/src
    ${PROJECT_SOURCE_DIR}/sdk/tool
    )

# 包含这些目录下的.cc文件
aux_source_directory(${PROJECT_SOURCE_DIR}/sdk/src SRC_BASE)

# 设置编译选项
set(CXX_FLAGS
    -g
    -Wall
    -std=c++11
    )

# 生成动态库 tiny_network
add_library(sdk SHARED 
            ${SRC_BASE}
            )

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)