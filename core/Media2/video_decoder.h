#pragma once

#include "decoder_interface.h"
#include "context.h"

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

class VideoDecoder : public DecoderInterface {
public:
    enum class ErrorCode {
        SUCCESS = 0,
        OTHER_ERROR = 1,
    };

    using FrameCallback = std::function<void(AVFrame* frame)>;

    VideoDecoder(std::shared_ptr<Context> context);

    // Interface
    std::shared_ptr<Error> Prepare() override; 

    std::shared_ptr<Error> Close() override;

    std::shared_ptr<Error> Decode() override;

    ~VideoDecoder();

    void SetFrameCallback(FrameCallback callback) {
        frame_callback_ = callback;
    }

private:
    std::shared_ptr<Error> DecodeInner();

    FrameCallback frame_callback_;
    AVCodecContext* codec_context_ = nullptr;
    AVFrame* frame_ = nullptr;
    AVPacket* packet_ = nullptr;
    std::shared_ptr<Context> context_;

    int64_t start_pts_ = AV_NOPTS_VALUE;
    int64_t next_pts_ = AV_NOPTS_VALUE;
    AVRational start_pts_tb_;
    AVRational next_pts_tb_;

    int finished_ = 0;
    int pkt_serial_ = -1;
    int packet_pending_ = 0;
    int recoder_pts_ = -1;

    std::shared_ptr<PacketWrapper> packet_wrapper_;
    std::shared_ptr<FrameWrapper> frame_wrapper_;
};

} // namespace tool