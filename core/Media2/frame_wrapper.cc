#include "frame_wrapper.h"

namespace tool {
    // 创建空帧
    std::shared_ptr<FrameWrapper> FrameWrapper::Create() {
        return std::make_shared<FrameWrapper>(av_frame_alloc());
    }

    // 从AVFrame深拷贝
    std::shared_ptr<FrameWrapper> FrameWrapper::Clone(AVFrame* src) {
        AVFrame* frame = av_frame_alloc();
        av_frame_ref(frame, src);  // 关键：引用计数管理
        return std::make_shared<FrameWrapper>(frame);
    }

    // 转移所有权
    std::shared_ptr<FrameWrapper> FrameWrapper::Move(AVFrame** src) {
        AVFrame* frame = av_frame_alloc();
        av_frame_move_ref(frame, *src);  // 转移引用
        return std::make_shared<FrameWrapper>(frame);
    }
}