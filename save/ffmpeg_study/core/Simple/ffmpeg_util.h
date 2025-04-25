#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Other/error.h"
#include "Log/logger.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#ifdef __cplusplus
};
#endif

namespace tool
{

enum class MediaErrorCode {
    kSuccess = 0,
    kError = -1,
};

class MediaUtil {
public:
    struct Config {
        std::string filename;
        std::string dest_filename_;
        bool use_find_best_stream = true;
        bool use_copy_file = false;
    };

    MediaUtil(const Config& config);

    ~MediaUtil();

    std::shared_ptr<Error> Prepare();

    std::shared_ptr<Error> Process();

    std::shared_ptr<Error> Seek(int64_t timestamp, int stream_index);

private:
    std::shared_ptr<Error> PrepareCore();
    std::shared_ptr<Error> PrepareDecode();

    Config config_;
    AVFormatContext* format_context_ = nullptr;
    AVFormatContext* out_format_context_ = nullptr; // 输出文件的封装器实例
    int video_stream_index_ = -1;
    int audio_stream_index_ = -1;
};

} // namespace tool


