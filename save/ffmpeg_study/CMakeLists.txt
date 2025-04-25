cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "/home/shangyizhou/code/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(ffmpeg_project)

option(FFMPEG_BUILD_CORE "whether build core" ON)
option(FFMPEG_BUILD_EXAMPLE "whether build example" ON)
option(FFMPEG_BUILD_CHAPTER "whether build example" ON)

list(APPEND CMAKE_PREFIX_PATH "/home/shangyizhou/code/cpp/vcpkg/installed/x64-linux")
find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message(STATUS "FFMPEG version: ${FFMPEG_VERSION}")
    message(STATUS "FFMPEG includes: ${FFMPEG_INCLUDE_DIRS}")
    message(STATUS "FFMPEG libraries: ${FFMPEG_LIBRARIES}")
endif()

find_package(spdlog CONFIG REQUIRED)
if(TARGET spdlog::spdlog)
    # 获取目标属性（现代CMake正确方式）
    get_target_property(SPDLOG_INCLUDES spdlog::spdlog INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(SPDLOG_LIBS spdlog::spdlog IMPORTED_LOCATION_RELEASE)  # 或 INTERFACE_LINK_LIBRARIES
    
    message(STATUS "ACTUAL spdlog includes: ${SPDLOG_INCLUDES}")
    message(STATUS "ACTUAL spdlog lib: ${SPDLOG_LIBS}")
endif()

if(FFmpeg_FOUND)
  target_include_directories(main PRIVATE ${FFMPEG_INCLUDE_DIRS})
  target_link_directories(main PRIVATE ${FFMPEG_LIBRARY_DIRS})
  target_link_libraries(main PRIVATE ${FFMPEG_LIBRARIES})
endif()

option(FFMPEG_BUILD_CORE "whether build core" ON)
option(FFMPEG_BUILD_EXAMPLE "whether build example" ON)
option(FFMPEG_BUILD_CHAPTER "whether build example" ON)

if (FFMPEG_BUILD_CORE)
    add_subdirectory(core)
endif()

if (FFMPEG_BUILD_CHAPTER)
    # add_subdirectory(chapter)
endif()

if (FFMPEG_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()