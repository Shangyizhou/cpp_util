#include "video_decoder.h"

namespace tool {
    VideoDecoder::VideoDecoder(std::shared_ptr<Context> context) {
        context_ = context;
    }
    
    VideoDecoder::~VideoDecoder() {
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
    
    std::shared_ptr<Error> VideoDecoder::Prepare() {
        codec_context_ = avcodec_alloc_context3(nullptr);
        avcodec_parameters_to_context(codec_context_, context_->video_avcodec_param_);
        
        frame_ = av_frame_alloc();
        packet_ = av_packet_alloc();
    
        if (!codec_context_ || !frame_ || !packet_) {
            LOG_ERROR("Failed to allocate codec context or frame");
            return Error::NewToolError(static_cast<int>(ErrorCode::OTHER_ERROR), "Failed to allocate codec context or frame");
        }
        
        // TODO: 提取出一个 StreamInfo 结构
        codec_context_->codec_id = context_->video_avcodec_param_->codec_id;
        codec_context_->width = context_->video_avcodec_param_->width;
        codec_context_->height = context_->video_avcodec_param_->height;
        codec_context_->time_base = context_->video_time_base_;
    
        // Open the codec
        AVCodec* codec = (AVCodec*) avcodec_find_decoder(codec_context_->codec_id);
        if (!codec) {
            LOG_ERROR("Codec not found");
            return Error::NewToolError(static_cast<int>(ErrorCode::OTHER_ERROR), "Codec not found");
        }
    
        if (avcodec_open2(codec_context_, codec, nullptr) < 0) {
            LOG_ERROR("Failed to open codec");
            return Error::NewToolError(static_cast<int>(ErrorCode::OTHER_ERROR), "Failed to open codec");
        }
    
        LOG_INFO("VideoDecoder Prepare Success");
    } 

    std::shared_ptr<Error> VideoDecoder::DecodeInner() {
        int ret = 0;
        do {

            /**
             * 1. 从 packet_queue 中取出 AVPacket 
             */
            do {
                if (context_->packet_queue_.IsEmpty()) {
                    // TODO: 唤醒Demuxer，阻塞情况 1.读完文件，等待解码线程解码完毕 2.packet_queue过多，需要阻塞等待
                    LOG_WARN("PacketQueue is empty");
    
                }
                
                bool is_pending = false;
                if (is_pending) {
                    // TODO: 等待补充暂停情况
                    LOG_INFO("Decoder is pending");
                    continue;
                }
                
                context_->packet_queue_.Pop(packet_wrapper_);
                if (!packet_wrapper_) {
                    LOG_ERROR("Failed to pop packet from queue");
                    continue;
                }

                if (context_->packet_queue_.IsEmpty()) {
                    LOG_INFO("Received empty packet");
                    continue;
                }
                
                break;
    
            } while (1);
            
            /**
             * 2. 发送 AVPacket 到解码器
             */
            if (avcodec_send_packet(codec_context_, packet_wrapper_->GetRall()) == AVERROR(EAGAIN)) {
                packet_pending_ = 1;
            }

            /**
             * 3. 从解码器中取出 AVFrame
             */
            do {
                AVFrame *frame = av_frame_alloc();
                ret = avcodec_receive_frame(codec_context_, frame);
                if (ret >= 0) {
                    if (AVMEDIA_TYPE_VIDEO == codec_context_->codec_type) {
                        if (start_pts_ == -1) {
                            frame->pts = frame->best_effort_timestamp;
                        } else if (!recoder_pts_) {
                            frame->pts = frame->pkt_dts;
                        }
                    } else if (AVMEDIA_TYPE_AUDIO == codec_context_->codec_type) {
                        // TODO: 音频的 pts 计算
                    }
                    LOG_INFO("avcodec_receive_frame Success");
                    auto framePtr = FrameWrapper::Move(&frame);
                    context_->frame_queue_.Push(framePtr);
                } else if (ret == AVERROR(EAGAIN)) {
                    // EAGAIN 表示解码器需要更多数据才能继续解码
                    packet_pending_ = 1;
                    LOG_INFO("Decoder needs more data");
                } else if (ret == AVERROR_EOF) {
                    // EOF 表示解码器已经完成解码
                    finished_ = 1;
                    LOG_INFO("Decoder finished");
                } else {
                    LOG_ERROR("Error receiving frame from decoder");
                }
    
            } while (ret != AVERROR(EAGAIN)); // EAGAIN 表示解码器需要更多数据才能继续解码

        } while (1);

    }
    
    std::shared_ptr<Error> VideoDecoder::Decode() {
        DecodeInner();
        
    }
    
    std::shared_ptr<Error> VideoDecoder::Close() {
        return nullptr;
    }

}