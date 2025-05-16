add_executable(player_github main.cc)
target_link_libraries(player_github tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(player_github PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)