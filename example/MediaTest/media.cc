#include "Media2/video_decoder.h"
#include "Media2/demuxer.h"
#include "Media2/context.h"

#include <iostream>
#include <thread>


int main() {
    auto context = std::make_shared<tool::Context>();
    context->input_filename_ = "/home/shangyizhou/code/cpp_util/video/BigBuckBunny_320x180.mp4";
    
    tool::Demuxer demuxer(context);
    tool::VideoDecoder decoder(context);

    demuxer.Prepare();

    auto error = decoder.Prepare();
    if (error) {
        LOG_ERROR("decode prepare error");
        return -1;
    }

    LOG_INFO("prepare success");
    std::thread t1([&](){
        demuxer.Process();
    });

    std::thread t2([&](){
        decoder.Decode();
    });

    t1.join();
    t2.join();
    
    return 0;
}