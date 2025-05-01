#pragma once

#include <memory>
#include <functional>

#include "frame_wrapper.h"
#include "Thread/thread_safe_queue.h"

namespace tool
{

class FrameQueue {
public:
    using FramePtr = std::shared_ptr<FrameWrapper>;

    FrameQueue() {}

    ~FrameQueue();

    void Push(FramePtr packet_ptr);

    void Pop(FramePtr& packet_ptr);

    void SetAbort() { is_abort_ = true; }

    int Count() const;

    void Destroy();

private:
    tool::ThreadSafeQueue<FramePtr> frame_queue_;
    std::atomic<bool> is_abort_ = false;
    
    int64_t duration_ = 0;
    int serial_ = 0;
};
    
} // namespace tool
