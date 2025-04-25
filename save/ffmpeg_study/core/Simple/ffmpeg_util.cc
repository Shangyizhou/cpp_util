#include "ffmpeg_util.h"

namespace tool {

MediaUtil::MediaUtil(const Config& config) {
    config_ = config;
    format_context_ = nullptr;
    video_stream_index_ = -1;
    audio_stream_index_ = -1;
}

MediaUtil::~MediaUtil() {
    if (format_context_) {
        avformat_close_input(&format_context_); // 关闭输入流
        avformat_free_context(format_context_); // 释放封装器的实例
        format_context_ = nullptr;
    }

    if (out_format_context_) {
        avio_close(out_format_context_->pb); // 关闭输出流
        avformat_free_context(out_format_context_); // 释放封装器的实例
        out_format_context_ = nullptr;
    }
}

std::shared_ptr<Error> MediaUtil::Prepare() {
    auto error = PrepareCore();
    if (error) {
        LOG_ERROR("PrepareCore failed: {}", error->ToString());
        return error;
    }

    error = PrepareDecode();
    if (error) {
        LOG_ERROR("PrepareDecode failed: {}", error->ToString());
        return error;
    }

    return Error::kNoError;
}

std::shared_ptr<Error> MediaUtil::PrepareCore() {
    avformat_network_init();

    format_context_ = avformat_alloc_context();
    if (!format_context_) {
        LOG_ERROR("Failed to allocate AVFormatContext");
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "Failed to allocate AVFormatContext");
    }

    if (avformat_open_input(&format_context_, config_.filename.c_str(), nullptr, nullptr) < 0) {
        LOG_ERROR("Failed to open input file: {}", config_.filename);
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "Failed to open input file");
    }

    if (avformat_find_stream_info(format_context_, nullptr) < 0) {
        LOG_ERROR("Failed to find stream info");
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "Failed to find stream info");
    }

    if (config_.use_find_best_stream) {
        LOG_INFO("use_find_best_stream");
        // 找到视频流的索引
        video_stream_index_ = av_find_best_stream(format_context_, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (video_stream_index_ >= 0) {
            LOG_INFO("src_video stream index: {}", video_stream_index_);
        }
        AVStream *src_audio = nullptr;
        // 找到音频流的索引
        int audio_stream_index_ = av_find_best_stream(format_context_, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        if (audio_stream_index_ >= 0) {
            LOG_INFO("src_audio stream index: {}", audio_stream_index_);
        }
    } else {
        LOG_INFO("not use_find_best_stream");
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

    return Error::kNoError;
}

std::shared_ptr<Error> MediaUtil::PrepareDecode() {
    AVStream* src_video;
    AVStream* src_audio;
    if (video_stream_index_ >= 0) {
        src_video = format_context_->streams[video_stream_index_];
        LOG_INFO("src_video stream index: {}", video_stream_index_);
    }

    if (audio_stream_index_ >= 0) {
        src_audio = format_context_->streams[audio_stream_index_];
        LOG_INFO("src_audio stream index: {}", audio_stream_index_);
    }

    // 分配音视频文件的封装实例
    int ret = avformat_alloc_output_context2(&out_format_context_, nullptr, nullptr, config_.dest_filename_.c_str());
    if (ret < 0) {
        LOG_ERROR("Can't alloc output_file {}", config_.dest_filename_);
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "Can't alloc output_file");
    }
    // 打开输出流
    ret = avio_open(&out_format_context_->pb, config_.dest_filename_.c_str(), AVIO_FLAG_READ_WRITE);
    if (ret < 0) {
        LOG_ERROR("Can't open output_file {}", config_.dest_filename_);
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "Can't open output_file");
    }

    LOG_INFO("Success open output_file {}", config_.dest_filename_.c_str());
    if (video_stream_index_ >= 0) { // 源文件有视频流，就给目标文件创建视频流
        AVStream *dest_video = avformat_new_stream(out_format_context_, nullptr); // 创建数据流
        // 把源文件的视频参数原样复制过来
        avcodec_parameters_copy(dest_video->codecpar, src_video->codecpar);
        dest_video->time_base = src_video->time_base;
        dest_video->codecpar->codec_tag = 0;
    }
    if (audio_stream_index_ >= 0) { // 源文件有音频流，就给目标文件创建音频流
        AVStream *dest_audio = avformat_new_stream(out_format_context_, nullptr); // 创建数据流
        // 把源文件的音频参数原样复制过来
        avcodec_parameters_copy(dest_audio->codecpar, src_audio->codecpar);
        dest_audio->codecpar->codec_tag = 0;
    }
    ret = avformat_write_header(out_format_context_, nullptr); // 写文件头
    if (ret < 0) {
        LOG_ERROR("write file_header occur error {}", ret);
        return NewToolError(static_cast<int>(MediaErrorCode::kError), "write file_header occur error");
    }
    
    LOG_INFO("Success write file_header");
    return Error::kNoError;
}

std::shared_ptr<Error> MediaUtil::Process() {
    if (config_.use_copy_file) {
        LOG_INFO("Start copy file");

        AVPacket *packet = av_packet_alloc(); // 分配一个数据包
        int ret = 0;
        while (av_read_frame(format_context_, packet) >= 0) { // 轮询数据包
            // 有的文件视频流没在第一路，需要调整到第一路，因为目标的视频流默认第一路
            if (packet->stream_index == video_stream_index_) { // 视频包
                packet->stream_index = 0;
                ret = av_write_frame(out_format_context_, packet); // 往文件写入一个数据包
            } else { // 音频包
                packet->stream_index = audio_stream_index_<0 ? 0 : 1;
                ret = av_write_frame(out_format_context_, packet); // 往文件写入一个数据包
            }
            // 分类处理错误
            if (ret == AVERROR(EAGAIN)) {
                // 含义：输出缓冲区已满	
                // ​处理方案：等待后重试（适合实时场景）或丢弃当前包
                LOG_WARN("Output buffer full, retrying...");
                continue;  // 重试当前包
            } 
            else if (ret == AVERROR_EOF) {
                // 含义：输出文件已结束
                // ​处理方案：停止写入流程，关闭文件
                LOG_ERROR("Output file terminated unexpectedly");
                break;  // 终止循环
            }
            else if (ret == AVERROR(EINVAL)) {
                // 含义：无效参数（如流索引越界）
                // ​处理方案：检查并修正 packet->stream_index，必要时重新映射流
                LOG_ERROR("Invalid stream index {} -> {}", packet->stream_index);
                av_packet_unref(packet);
                continue;  // 跳过无效包
            }
            else if (ret == AVERROR(ENOSPC)) {
                // 含义：内存不足	
                // ​处理方案：释放资源或降低写入速度
                LOG_ERROR("Disk space exhausted!");
                break;  // 立即停止
            }
            if (ret < 0) {
                LOG_ERROR("write frame occur error {}", ret);
                break;
            }
            av_packet_unref(packet); // 清除数据包
        }
        av_write_trailer(out_format_context_); // 写文件尾
        
        av_packet_free(&packet); // 释放数据包资源
    }
    return  Error::kNoError;
}

}