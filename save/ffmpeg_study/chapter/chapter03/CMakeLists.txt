add_executable(timestamp timestamp.cc)
add_executable(copyfile copyfile.cc)
add_executable(mergeaudio mergeaudio.cc)
add_executable(mux264 mux264.cc)
add_executable(mergevideo mergevideo.cc)
add_executable(fps fps.cc)
add_executable(peelaudio peelaudio.cc)
add_executable(recode recode.cc)


target_link_libraries(timestamp ffmpeg_util)
target_link_libraries(copyfile ffmpeg_util)
target_link_libraries(mergeaudio ffmpeg_util)
target_link_libraries(mux264 ffmpeg_util)
target_link_libraries(mergevideo ffmpeg_util)
target_link_libraries(fps ffmpeg_util)
target_link_libraries(peelaudio ffmpeg_util)
target_link_libraries(recode ffmpeg_util)


set(CHAPTER_02_TARGET 
timestamp
copyfile
mergeaudio
mux264
mergevideo
fps
peelaudio
recode)

# 设置这些可执行文件最终在 {project}/bin/chapter03 目录下，避免污染全局
set_target_properties(${CHAPTER_02_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/chapter03")