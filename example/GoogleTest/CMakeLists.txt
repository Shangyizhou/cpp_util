add_executable(google_test google_test.cc)
target_link_libraries(google_test tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(google_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)