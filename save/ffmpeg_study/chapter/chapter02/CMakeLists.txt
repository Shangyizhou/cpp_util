add_executable(read read.cc)
add_executable(look look.cc)
add_executable(code code.cc)
add_executable(write write.cc)
add_executable(para para.cc)

target_link_libraries(read  ffmpeg_util)
target_link_libraries(look  ffmpeg_util)
target_link_libraries(code  ffmpeg_util)
target_link_libraries(write ffmpeg_util)
target_link_libraries(para  ffmpeg_util)

set(CHAPTER_02_TARGET 
    read
    look
    code
    write
    para)

# 设置这些可执行文件最终在 {project}/bin/chapter02 目录下，避免污染全局
set_target_properties(${CHAPTER_02_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/chapter02"
)