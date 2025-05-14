add_executable(player2 player.cc)
target_link_libraries(player2 tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(player2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)