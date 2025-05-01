#include "packet_wrapper.h"

namespace tool {

    std::shared_ptr<PacketWrapper> PacketWrapper::NewPacket() {
        return std::make_shared<PacketWrapper>(av_packet_alloc());
    }

    // 从现有 AVPacket 深拷贝
    std::shared_ptr<PacketWrapper> PacketWrapper::ClonePacket(AVPacket *src) {
        AVPacket *pkt = av_packet_alloc();
        av_packet_ref(pkt, src);
        return std::make_shared<PacketWrapper>(pkt);
    }

    std::shared_ptr<PacketWrapper> PacketWrapper::Move(AVPacket* src) {
        AVPacket* pkt = av_packet_alloc();
        av_packet_move_ref(pkt, src);  // 转移所有权，src 被清空
        return std::make_shared<PacketWrapper>(pkt);
    }
}