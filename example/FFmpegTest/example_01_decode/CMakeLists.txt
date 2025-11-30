add_executable(decode main.cc)
target_link_libraries(decode tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(decode PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/ffmpeg_example"
)