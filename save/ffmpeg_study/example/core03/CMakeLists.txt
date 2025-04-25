add_executable(media_util simple.cc)

target_link_libraries(media_util ffmpeg_util)

set_target_properties(media_util PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/example"
)