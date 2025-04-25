#include "decoder.h"
#include <iostream>

namespace tool {

void CheckH264Support() {
    // 检查解码器
    const AVCodec* dec = avcodec_find_decoder(AV_CODEC_ID_H264);
    printf("H.264 decoder: %s\n", dec ? "YES" : "NO");
    
    // 检查编码器
    const AVCodec* enc = avcodec_find_encoder(AV_CODEC_ID_H264);
    printf("H.264 encoder: %s\n", enc ? "YES" : "NO");
}

Decoder::Decoder(AVStream* av_stream) {
    codec_context_ = avcodec_alloc_context3(nullptr);  // 先创建空上下文
    av_stream_ = av_stream;
}

Decoder::~Decoder() {
    if (codec_context_) {
        avcodec_free_context(&codec_context_);
        codec_context_ = nullptr;
    }
}

void Decoder::Prepare() {
    std::cout << "------------" << std::endl;
    CheckH264Support();
    std::cout << "------------" << std::endl;

    // Allocate codec context and frame
    codec_context_ = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codec_context_, av_stream_->codecpar);
    
    if (!codec_context_) {
        throw std::runtime_error("Failed to allocate codec context or frame");
        return;
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
        throw std::runtime_error("Codec not found");
        return;
    }

    if (avcodec_open2(codec_context_, codec, nullptr) < 0) {
        throw std::runtime_error("Could not open codec");
        return;
    }

    std::cout << "Prepare Successful" << std::endl;
}

void Decoder::Decode(AVPacket* pkt) {
    while (true) {
        if (!pkt->data && pkt->size == 0) {
            std::cerr << "这是一个空包，发送空包刷新解码器" << std::endl;
            avcodec_send_packet(codec_context_, pkt);
            return;
        }
        if (!pkt->data || pkt->size < 0) {
            std::cerr << "pkt data nullptr" << std::endl;
            return;
        }

        if (!codec_context_ || !codec_context_->codec) {
            std::cerr << "Decoder not initialized!" << std::endl;
            return;
        }

        std::cout << "avcodec_send_packet packet" << std::endl;
        // 把未解压的数据包发给解码器实例
        if (avcodec_send_packet(codec_context_, pkt) < 0) {
            std::cout << "Error sending packet to decoder" << std::endl;
            av_packet_unref(pkt);
            return;
        }

        std::cout << "Send packet to decoder" << std::endl;
        
        do {
            // 从解码器实例获取还原后的数据帧
            // ​0	成功，数据包已被解码器接受。
            // ​AVERROR(EAGAIN)	解码器缓冲区已满，需要先调用 avcodec_receive_frame() 取出帧。
            // ​AVERROR_EOF	解码器已刷新（发送了空包），不能再接受新数据包。
            // ​其他负值	错误（如无效参数、解码器未初始化等）。
            AVFrame* frame = av_frame_alloc();
            int ret = avcodec_receive_frame(codec_context_, frame);
            if (ret == 0) {
                std::cout << "ret == 0 frame_callback" << std::endl;
                if (frame_callback_) {
                    frame_callback_(frame);
                }
                av_frame_unref(frame);
            }
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                if (ret == AVERROR_EOF) {
                    std::cout << "Decoder EOF" << std::endl;
                } else if (ret == AVERROR(EAGAIN)) {
                    std::cout << "Decoder EAGAIN" << std::endl;
                    av_frame_unref(frame);
                    break; // 需要更多输入数据包，继续调用avcodec_send_packet输入数据包
                }
            } else if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "decode frame occur error %d.\n", ret);
                break;
            }

        } while(true);

        av_packet_unref(pkt);
    }
}

} // namespace tool