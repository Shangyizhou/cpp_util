add_executable(player3 main.cc)
target_link_libraries(player3 tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(player3 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)