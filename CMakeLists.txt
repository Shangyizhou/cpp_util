cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_TOOLCHAIN_FILE "/home/shangyizhou/code/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
project(MyProject)

option(TOOL_BUILD_CORE "whether build core" ON)
option(TOOL_BUILD_EXAMPLE "whether build example" ON)

# REQUIRED 设置之后如果没有找到则会退出
list(APPEND CMAKE_PREFIX_PATH "/home/shangyizhou/code/cpp/vcpkg/installed/x64-linux")
find_package(spdlog CONFIG REQUIRED)

if(TARGET spdlog::spdlog)
    # 获取目标属性（现代CMake正确方式）
    get_target_property(SPDLOG_INCLUDES spdlog::spdlog INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SPDLOG_LIBS spdlog::spdlog IMPORTED_LOCATION_RELEASE)  # 或 INTERFACE_LINK_LIBRARIES
    
    message(STATUS "ACTUAL spdlog includes: ${SPDLOG_INCLUDES}")
    message(STATUS "ACTUAL spdlog lib: ${SPDLOG_LIBS}")
endif()

find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message(STATUS "FFMPEG version: ${FFMPEG_VERSION}")
    message(STATUS "FFMPEG includes: ${FFMPEG_INCLUDE_DIRS}")
    message(STATUS "FFMPEG libraries: ${FFMPEG_LIBRARIES}")
endif()


find_package(protobuf CONFIG REQUIRED)

if(protobuf_FOUND)
    message(STATUS "protobuf version: ${protobuf_VERSION}")
    message(STATUS "protobuf includes: ${protobuf_INCLUDE_DIRS}")
    message(STATUS "protobuf libraries: ${protobuf_LIBRARIES}")
endif()

find_package(GTest CONFIG REQUIRED)

if(GTest_FOUND)
    message(STATUS "GTest version: ${GTest_VERSION}")
    message(STATUS "GTest includes: ${GTEST_INCLUDE_DIRS}")
    message(STATUS "GTest libraries: ${GTEST_LIBRARIES}")
endif()

find_package(miniz CONFIG REQUIRED)

if(TOOL_BUILD_CORE)
    message(STATUS "Build core")
    add_subdirectory(core)
endif()

if(TOOL_BUILD_EXAMPLE)
    message(STATUS "Build example")
    add_subdirectory(example)
endif()