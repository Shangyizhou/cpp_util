add_executable(thread_test thread_test.cc)
target_link_libraries(thread_test tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(thread_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)