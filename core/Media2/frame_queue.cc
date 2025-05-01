#include "frame_queue.h"

namespace tool {

FrameQueue::~FrameQueue() {

}

void FrameQueue::Push(FramePtr packet_ptr) {
    frame_queue_ .Push(packet_ptr);
}

void FrameQueue::Pop(FramePtr& packet_ptr) {
    frame_queue_ .Pop(packet_ptr);
}

int FrameQueue::Count() const {
    return frame_queue_ .Size();
}

void FrameQueue::Destroy() {
    FramePtr ptr;
    while (!frame_queue_ .Empty()) {
        auto flag = frame_queue_ .TryPop(ptr);
    }
}

}