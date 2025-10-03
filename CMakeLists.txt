cmake_minimum_required(VERSION 3.22.0)

cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_FLAGS
    -g
    -Wall
    -O3
    -std=c++14
    )

# cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[你的vcpkg安装路径]/scripts/buildsystems/vcpkg.cmake
set(CMAKE_TOOLCHAIN_FILE "/home/shangyizhou/code/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
project(MyProject)

option(TOOL_BUILD_CORE "whether build core" ON)
option(TOOL_BUILD_EXAMPLE "whether build example" ON)

# REQUIRED 设置之后如果没有找到则会退出
# list(APPEND CMAKE_PREFIX_PATH "/home/shangyizhou/code/cpp/vcpkg/installed/x64-linux")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

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
if (miniz_FOUND)
    message(STATUS "miniz version: ${miniz_VERSION}")
    message(STATUS "miniz includes: ${MINIZ_INCLUDE_DIRS}")
    message(STATUS "miniz libraries: ${MINIZ_LIBRARIES}")
endif()

find_package(SDL2 CONFIG REQUIRED)
if(SDL2_FOUND)
    message(STATUS "SDL2 version: ${SDL_VERSION}")
    message(STATUS "SDL2 includes: ${SDL_INCLUDE_DIRS}")
    message(STATUS "SDL2 libraries: ${SDL_LIBRARIES}")
endif()

find_package(SqliteOrm CONFIG REQUIRED)
if(SqliteOrm_FOUND)
    message(STATUS "SqliteOrm found: ${SqliteOrm_FOUND}")
    message(STATUS "SqliteOrm version: ${SqliteOrm_VERSION}")
    message(STATUS "SqliteOrm config dir: ${SqliteOrm_DIR}")

    # 获取SqliteOrm目标属性
    if(TARGET sqlite_orm::sqlite_orm)
        get_target_property(SQLITE_ORM_INCLUDES sqlite_orm::sqlite_orm INTERFACE_INCLUDE_DIRECTORIES)
        get_target_property(SQLITE_ORM_LIBRARIES sqlite_orm::sqlite_orm IMPORTED_LOCATION_RELEASE)
        message(STATUS "SqliteOrm include directories: ${SQLITE_ORM_INCLUDES}")
        message(STATUS "SqliteOrm library location: ${SQLITE_ORM_LIBRARIES}")
    endif()
else()
    message(FATAL_ERROR "SqliteOrm not found!")
endif()

if(TOOL_BUILD_CORE)
    message(STATUS "Build core")
    add_subdirectory(core)
endif()

if(TOOL_BUILD_EXAMPLE)
    message(STATUS "Build example")
    add_subdirectory(example)
endif()