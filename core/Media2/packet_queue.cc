#include "packet_queue.h"

namespace tool {

// PacketQueue::PacketPtr pkt = Packet::NewPacket();
// av_send_frame(ctx, pkt.get());
// queue.push(pkt)
void PacketQueue::Push(PacketPtr packet_ptr) {
    packet_ptr->serial_ = serial_;
    duration_ += packet_ptr->GetRall()->duration;
    packet_queue_.Push(packet_ptr);
    LOG_INFO("PacketQueue push Packet");
}

void PacketQueue::Pop(PacketPtr& packet_ptr) {
    if (is_abort_) {
        LOG_WARN("PacketQueue is abort");
        return;
    }
    packet_queue_.Pop(packet_ptr);
    LOG_INFO("PacketQueue pop Packet");
}

int PacketQueue::Size() const {
    LOG_INFO("PacketQueue count is {}", packet_queue_.Size());
    return packet_queue_.Size();
}

bool PacketQueue::IsEmpty() {
    return packet_queue_.Size() == 0;
}

void PacketQueue::Destroy() {
    tool::PacketQueue::PacketPtr ptr;
    while (!packet_queue_.Empty()) {
        auto flag = packet_queue_.TryPop(ptr);
    }
    LOG_INFO("all packet pop");
}

PacketQueue::~PacketQueue() {
    Destroy();
}

}