#include "Simple/ffmpeg_util.h"

int main() {
    tool::MediaUtil::Config config;
    config.filename = "/home/shangyizhou/code/cpp/tool/bin/video/video.mp4";
    config.dest_filename_ = "output_recode.mp4";
    config.use_copy_file = true;
    config.use_find_best_stream = true;

    tool::MediaUtil media_util(config);
    auto error = media_util.Prepare();
    if (error) {
        LOG_ERROR("Prepare failed: {}", error->ToString());
        return -1;
    }

    error = media_util.Process();
    if (error) {
        LOG_ERROR("Process failed: {}", error->ToString());
        return -1;
    }

    return 0;
}