#include "demuxer.h"
#include "Log/logger.h"

namespace tool
{


Demuxer::Demuxer(const std::string &filename) {
    filename_ = filename;
    format_context_ = nullptr;
    video_stream_index_ = -1;
    audio_stream_index_ = -1;
}

Demuxer::~Demuxer() {
    if (format_context_) {
        avformat_close_input(&format_context_);
        avformat_free_context(format_context_);
        format_context_ = nullptr;
    }
}

void Demuxer::Prepare() {
    avformat_network_init();

    format_context_ = avformat_alloc_context();
    if (!format_context_) {
        LOG_ERROR("Failed to allocate AVFormatContext");
        return;
    }

    if (avformat_open_input(&format_context_, filename_.c_str(), nullptr, nullptr) < 0) {
        LOG_ERROR("Failed to open input file");
        return;
    }

    if (avformat_find_stream_info(format_context_, nullptr) < 0) {
        LOG_ERROR("Failed to find stream info");
        return;
    }

    for (unsigned int i = 0; i < format_context_->nb_streams; ++i) {
        AVStream *stream = format_context_->streams[i];
        AVCodecParameters *codec_params = stream->codecpar;

        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_index_ == -1) {
            video_stream_index_ = i;
        } else if (codec_params->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_index_ == -1) {
            audio_stream_index_ = i;
        }
    }
}

void Demuxer::Process() {
    AVPacket packet;
    while (av_read_frame(format_context_, &packet) >= 0) {
        if (packet.stream_index == video_stream_index_) {
            // Process video packet
            if (video_callback_) {
                video_callback_(&packet);
            }
        } else if (packet.stream_index == audio_stream_index_) {
            // todo: Process audio packet
        }
        av_packet_unref(&packet);
    }
}

void Demuxer::Seek(int64_t timestamp, int stream_index) {
    if (format_context_ == nullptr) {
        LOG_ERROR("Format context is not initialized");\
        return;
    }

    int64_t seek_target = av_rescale_q(timestamp, AV_TIME_BASE_Q, format_context_->streams[stream_index]->time_base);
    if (av_seek_frame(format_context_, stream_index, seek_target, AVSEEK_FLAG_BACKWARD) < 0) {
        LOG_ERROR("Failed to seek to timestamp: {}", timestamp);
        return;
    }
}

AVStream* Demuxer::GetVideoStream() {
    if (video_stream_index_ == -1) {
        LOG_ERROR("No video stream found");
        return;
    }
    return format_context_->streams[video_stream_index_];
}

}