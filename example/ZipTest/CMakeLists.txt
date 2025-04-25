add_executable(zip zip.cc)
target_link_libraries(zip tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(zip PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/"
)