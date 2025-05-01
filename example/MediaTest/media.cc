#include "Media/demuxer.h"
#include "Media/decoder.h"

#include <iostream>

int main() {
    tool::Demuxer demuxer("/home/shangyizhou/code/cpp/tool/bin/video/video.mp4");
    demuxer.Prepare();
    

    auto video_stream = demuxer.GetVideoStream();
    tool::Decoder decoder(video_stream);
    decoder.SetFrameCallback([](AVFrame *frame) {
        // Process decoded frame
        std::cout << "Decoded frame with width: " << frame->width << ", height: " << frame->height << std::endl;
    });

    demuxer.SetVideoCallback([&decoder](AVPacket* packet) {
        // Process video packet
        std::cout << "Processing video packet with size: " << packet->size << std::endl;
        decoder.Decode(packet);
    });

    demuxer.Process();
    
    return 0;
}