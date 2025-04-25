#include "decoder.h"
#include "Log/logger.h"

namespace tool {
    
Decoder::Decoder(const AVStream* av_stream) {
    codec_context_ = avcodec_alloc_context3(nullptr);  // 先创建空上下文
    frame_ = av_frame_alloc();
    packet_ = av_packet_alloc();

    if (!frame_ || !packet_) {
        LOG_ERROR("Failed to allocate frame or packet");
        return;
    }
}

Decoder::~Decoder() {
    if (codec_context_) {
        avcodec_free_context(&codec_context_);
        codec_context_ = nullptr;
    }
    if (frame_) {
        av_frame_free(&frame_);
        frame_ = nullptr;
    }
    if (packet_) {
        av_packet_free(&packet_);
        packet_ = nullptr;
    }
}

void Decoder::Prepare() {
    // Allocate codec context and frame
    codec_context_ = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codec_context_, av_stream_->codecpar);
    
    frame_ = av_frame_alloc();
    packet_ = av_packet_alloc();

    if (!codec_context_ || !frame_ || !packet_) {
        LOG_ERROR("Failed to allocate codec context or frame");
    }

    // Set up codec context parameters (e.g., codec ID, width, height, etc.)
    // This is just an example; you would typically get these from a demuxer
    codec_context_->codec_id = av_stream_->codecpar->codec_id;
    codec_context_->width = av_stream_->codecpar->width;
    codec_context_->height = av_stream_->codecpar->height;
    codec_context_->time_base = av_stream_->time_base;
    // codec_context_->pix_fmt = AV_PIX_FMT_YUV420P; // Example pixel format
    // codec_context_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    // codec_context_->thread_count = 4; // Example thread count
    // codec_context_->thread_type = FF_THREAD_FRAME;
    // codec_context_->max_b_frames = 2; // Example max B-frames

    // Open the codec
    AVCodec* codec = (AVCodec*) avcodec_find_decoder(codec_context_->codec_id);
    if (!codec) {
        LOG_ERROR("Codec not found");
        return;
    }

    if (avcodec_open2(codec_context_, codec, nullptr) < 0) {
        LOG_ERROR("Failed to open codec");
        return;
    }
}

void Decoder::Decode(AVPacket* pkt) {
    while (true) {
        if (!packet_->data || packet_->size <= 0) {
            LOG_ERROR("Invalid packet data");
            break;
        }

        if (!codec_context_ || !codec_context_->codec) {
            LOG_ERROR("Codec context not initialized");
            return;
        }

        std::cout << "avcodec_send_packet packet" << std::endl;
        // Send the packet to the decoder
        if (avcodec_send_packet(codec_context_, packet_) < 0) {
            LOG_ERROR("Error sending packet to decoder");
            av_packet_unref(packet_);
            continue; // Error sending packet
        }

        LOG_INFO("avcodec_send_packet packet");

        // Receive the decoded frame
        while (avcodec_receive_frame(codec_context_, frame_) >= 0) {
            LOG_INFO("avcodec_receive_frame frame");
            if (frame_callback_) {
                frame_callback_(frame_);
            }
            av_frame_unref(frame_);
        }

        av_packet_unref(packet_);
    }
}

} // namespace tool