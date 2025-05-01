#pragma onces

#include <memory>

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#ifdef __cplusplus
};
#endif

namespace tool {

class PacketWrapper {
public:
    // 创建新的空 AVPacket
    static std::shared_ptr<PacketWrapper> NewPacket();
    
    // 从现有 AVPacket 深拷贝
    static std::shared_ptr<PacketWrapper> ClonePacket(AVPacket *src);

    // 从现有 AVPacket 转移所有权
    static std::shared_ptr<PacketWrapper> Move(AVPacket* src);
    
    AVPacket* GetRall() const { return pkt_.get(); }
public:
    int serial_ = 0;

    // AVPacket 的释放器
    static void PacketDeleter(AVPacket* pkt) {
        if (pkt) {
            // 释放AVPacket结构体 av_packet_free 内部已经包含 av_packet_unref 减少引用计数
            av_packet_free(&pkt); 
        }
    }

    PacketWrapper(AVPacket *pkt) : pkt_(pkt, PacketDeleter) {}
    ~PacketWrapper() {}

    std::shared_ptr<AVPacket> pkt_; // packet 数据
};

}