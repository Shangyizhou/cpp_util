#pragma once

#include "Thread/thread_safe_queue.h"
#include "packet_wrapper.h"

#include <atomic>

namespace tool {

class PacketQueue {
public:
    using PacketPtr = std::shared_ptr<PacketWrapper>;

    PacketQueue() {}

    ~PacketQueue();

    void Push(PacketPtr packet_ptr);

    void Pop(PacketPtr& packet_ptr);

    void SetAbort() { is_abort_ = true; }

    int Size() const;

    bool IsEmpty();

    void Destroy();

private:
    tool::ThreadSafeQueue<PacketPtr> packet_queue_;
    std::atomic<bool> is_abort_ = false;
    
    int64_t duration_ = 0;
    int serial_ = 0;

};

} // namespace tool

