add_executable(writeyuv writeyuv.cc)
add_executable(saveyuv saveyuv.cc)
add_executable(savejpg savejpg.cc)
add_executable(savejpg_sws savejpg_sws.cc)
add_executable(savepng savepng.cc)
add_executable(savebmp savebmp.cc)
add_executable(image2video image2video.cc)
add_executable(savegif savegif.cc)

target_link_libraries(writeyuv ffmpeg_util)
target_link_libraries(saveyuv ffmpeg_util)
target_link_libraries(savejpg ffmpeg_util)
target_link_libraries(savejpg_sws ffmpeg_util)
target_link_libraries(savepng ffmpeg_util)
target_link_libraries(savebmp ffmpeg_util)
target_link_libraries(image2video ffmpeg_util)
target_link_libraries(savegif ffmpeg_util)

set(CHAPTER_04_TARGET 
writeyuv
saveyuv
savejpg
savejpg_sws
savepng
savebmp
image2video
savegif)

# 设置这些可执行文件最终在 {project}/bin/chapter04 目录下，避免污染全局
set_target_properties(${CHAPTER_04_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/chapter04")