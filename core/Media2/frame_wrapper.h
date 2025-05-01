#pragma once

#include <memory>
#include <functional>

extern "C" {
#include <libavutil/frame.h>
}

namespace tool {

class FrameWrapper {
public:
    explicit FrameWrapper(AVFrame* frame) : frame_(frame, FrameDeleter) {}

    // 创建空帧
    static std::shared_ptr<FrameWrapper> Create();

    // 从AVFrame深拷贝
    static std::shared_ptr<FrameWrapper> Clone(AVFrame* src);

    // 转移所有权
    static std::shared_ptr<FrameWrapper> Move(AVFrame** src);

    // 获取原始指针（谨慎使用）
    AVFrame* GetRaw() const { return frame_.get(); }

    // 封装常用操作
    int width() const { return frame_ ? frame_->width : 0; }
    int height() const { return frame_ ? frame_->height : 0; }

private:
    static void FrameDeleter(AVFrame* frame) {
        av_frame_free(&frame);  // 自动处理引用计数
    }

    std::shared_ptr<AVFrame> frame_;
};

}