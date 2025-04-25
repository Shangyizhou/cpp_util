#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#ifdef __cplusplus
};
#endif

#include <string>
#include <functional>

namespace tool {

class Decoder {
public:
    using FrameCallback = std::function<void(AVFrame* frame)>;

    Decoder(AVStream* av_stream);

    ~Decoder();

    void Prepare();

    void Decode(AVPacket* pkt);

    void OnFrameCallback(FrameCallback callback) {
        frame_callback_ = callback;
    }

private:
    FrameCallback frame_callback_;
    AVCodecContext* codec_context_ = nullptr;
    AVStream* av_stream_ = nullptr;
};

} // namespace tool