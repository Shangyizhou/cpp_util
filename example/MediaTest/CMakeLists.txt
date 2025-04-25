add_executable(media media.cc)
target_link_libraries(media tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(media PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)