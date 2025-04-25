#pragma once

#include <string>
#include <vector>
#include <functional>

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

class Demuxer {
public:
    using DemuxerCallback = std::function<void(AVPacket *packet)>;

    Demuxer(const std::string &filename);
    ~Demuxer();

    void SetVideoCallback(DemuxerCallback callback) {
        video_callback_ = callback;
    }

    void Prepare();

    void Process();

    void Seek(int64_t timestamp, int stream_index);

    AVStream* GetVideoStream();

private:
    DemuxerCallback video_callback_;
    AVFormatContext* format_context_ = nullptr;
    std::string filename_;
    int video_stream_index_ = -1;
    int audio_stream_index_ = -1;
};

} // namespace tool
