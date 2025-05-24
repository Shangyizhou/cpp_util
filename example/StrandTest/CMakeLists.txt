add_executable(strand strand.cc)
target_link_libraries(strand tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(strand PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)