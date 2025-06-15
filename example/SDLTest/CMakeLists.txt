add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
add_link_options(-fsanitize=address)
message(STATUS "AddressSanitizer enabled")

add_executable(video_player main.cc video_player.cc)
target_link_libraries(video_player tool)
# 设置可执行文件输出到项目根目录下的 bin 文件夹
set_target_properties(video_player PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

