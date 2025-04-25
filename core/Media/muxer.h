#pragma onces


#include <string>
#include <vector>
#include <functional>

#include "Foundation/error.h"
#include "Log/logger.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#ifdef __cplusplus
};
#endif

namespace tool
{ 

class Muxer
{
public:
    enum class ErrorCode {
        SUCCESS = 0,
        OTHER_ERROR = 1,
    };

    using PacketCallback = std::function<void(AVPacket*, std::shared_ptr<Error>)>;

    Muxer(const std::string& dst_name);
    
    ~Muxer();
    
    // 向容器添加媒体流
    std::shared_ptr<Error> AddStream();

    // 写入文件头
    std::shared_ptr<Error> WriteHeader();

    // 写入媒体数据包
    std::shared_ptr<Error> WritePacket();

    // 写入文件尾
    std::shared_ptr<Error> WriteTrailer();

    void SetOnPacketCallback(PacketCallback callback) {
        packet_callback_ = callback;
    }

private:
    PacketCallback packet_callback_;
    AVFormatContext *output_format_context_;
    std::string dst_name_;

};




} // namespace tool

