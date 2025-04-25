#include "Media/demuxer.h"
#include "Media/decoder.h"

#include <iostream>

void save_yuv() {
    // 1. 初始化解封装器（Demuxer）
    tool::Demuxer demuxer("/home/shangyizhou/code/cpp/tool/bin/video/video.mp4");
    demuxer.Prepare();

    // 2. 获取视频流并检查有效性
    auto video_stream = demuxer.GetVideoStream();
    
    // 3. 初始化解码器（Decoder）
    tool::Decoder decoder(video_stream);
    decoder.OnFrameCallback([](AVFrame* frame) {
        // 帧处理回调（示例：打印帧信息）
        std::cout << "Decoded frame: " << frame->width << "x" << frame->height << std::endl;
    });

    decoder.Prepare();

    // 4. 定义数据包处理回调（Lambda）
    auto packet_callback = [&decoder](AVPacket* packet) {
        if (!packet->data || packet->size <= 0) {
            std::cerr << "Invalid packet data!" << std::endl;
            exit(1);
        }
        std::cout << "Processing packet, size: " << packet->size << std::endl;
        decoder.Decode(packet); 
    };

    // 5. 设置回调并开始处理
    demuxer.OnPacketCallback(packet_callback);
    demuxer.Process();  
}

int main() {
    save_yuv();
    
    return 0;
}