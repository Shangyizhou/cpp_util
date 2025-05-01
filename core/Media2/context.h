#pragma once

#include "packet_queue.h"
#include "frame_queue.h"

namespace tool {

/**
 * 保存音视频的上下文信息
 */
struct Context {
    std::string input_filename_;
    PacketQueue packet_queue_;  
    FrameQueue frame_queue_;

    AVCodecParameters* video_avcodec_param_;
    AVCodecParameters* audio_avcodec_param_;

    AVRational video_time_base_;
    AVRational audio_time_base_;

    std::mutex pause_mutex_;
    std::condition_variable pause_cond_;
    std::atomic<bool> paused_ = false; // 暂停/恢复播放
    std::atomic<bool> stop_ = false;   // 停止播放
};

}