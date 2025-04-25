add_executable(thread_test thread_test.cc)
add_executable(thread_safe_queue thread_safe_queue.cc)
target_link_libraries(thread_test tool)
target_link_libraries(thread_safe_queue tool)

set(TARGET thread_test thread_safe_queue)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)