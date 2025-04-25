#include "muxer.h"

namespace tool {

Muxer::Muxer(const std::string& dst_name) {
    output_format_context_ = nullptr;
    dst_name_ = dst_name;

    avformat_alloc_output_context2(&output_format_context_, nullptr, nullptr, dst_name_.c_str());
    if (!output_format_context_) {
        LOG_ERROR("Could not create output format context");
        return;
    }
}

Muxer::~Muxer() {
    if (output_format_context_) {
        avformat_free_context(output_format_context_);
        output_format_context_ = nullptr;
    }
}

std::shared_ptr<Error> Muxer::AddStream() {
    /**
     * 有不同的处理方法，首先这里是直接针对容器格式增加一个媒体流，往往是新创建的多媒体文件格式
     * 如果我们想拷贝一个流，可以直接把源文件的视频参数原样复制过来 avcodec_parameters_copy(dest_video->codecpar, src_video->codecpar);
     * 如果涉及到转码，比如新文件是别的编码格式，或者是不同的分辨率等，那么我们需要创建一个新的流，并设置编码器参数，这个时候需要传入编码器ID
    */
    AVStream *stream = avformat_new_stream(output_format_context_, nullptr);
    if (!stream) {
        LOG_ERROR("Could not create new stream");
        return std::make_shared<Error>(ErrorCode::OTHER_ERROR, "Could not create new stream");
    }

    // Set stream parameters here (codec, bitrate, etc.)
    // For example, if you are using H.264 codec:
    

    return nullptr;
}

std::shared_ptr<Error> Muxer::WriteHeader() {
    int ret = avformat_write_header(output_format_context_, nullptr); // 写文件头
    if (ret < 0) {
        LOG_ERROR("Could not write file header and ret = {}", ret);
        return std::make_shared<Error>(ErrorCode::OTHER_ERROR, "Could not write file header");
    }
    return nullptr;
}

std::shared_ptr<Error> Muxer::WritePacket() {
    AVPacket *packet = av_packet_alloc(); // 分配一个数据包
    if (!packet) {
        LOG_ERROR("Could not allocate AVPacket");
        return std::make_shared<Error>(ErrorCode::OTHER_ERROR, "Could not allocate AVPacket");
    }

    if (packet_callback_) {
        packet_callback_(packet, nullptr); // 调用回调函数处理数据包
    }

    

    AVFrame *frame = av_frame_alloc(); // 分配一个数据帧

}

std::shared_ptr<Error> Muxer::WriteTrailer() {
    int ret = av_write_trailer(output_format_context_); // 写文件尾
    if (ret < 0) {
        LOG_ERROR("Could not write file trailer and ret = {}", ret);
        return std::make_shared<Error>(ErrorCode::OTHER_ERROR, "Could not write file trailer");
    }
    return nullptr;
}
}