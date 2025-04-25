#include <stdio.h>

// 之所以增加__cplusplus的宏定义，是为了同时兼容gcc编译器和g++编译器
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#ifdef __cplusplus
};
#endif

int main(int argc, char **argv) {
    const char *filename = "../fuzhou.mp4";
    if (argc > 1) {
        filename = argv[1];
    }
    AVFormatContext *fmt_ctx = NULL;
    // 打开音视频文件
    int ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file %s.\n", filename);
        return -1;
    }
    av_log(NULL, AV_LOG_INFO, "Success open input_file %s.\n", filename);
    // 查找音视频文件中的流信息
    ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find stream information.\n");
        return -1;
    }
    av_log(NULL, AV_LOG_INFO, "Success find stream information.\n");
    const AVInputFormat* iformat = fmt_ctx->iformat;
    av_log(NULL, AV_LOG_INFO, "format name is %s.\n", iformat->name);
    av_log(NULL, AV_LOG_INFO, "format long_name is %s\n", iformat->long_name);
    
    av_log(NULL, AV_LOG_INFO, "===== File Information =====\n");
    av_log(NULL, AV_LOG_INFO, "Format: %s (%s)\n", iformat->name, iformat->long_name);
    av_log(NULL, AV_LOG_INFO, "Duration: %.2f seconds\n", fmt_ctx->duration / 1000000.0);
    av_log(NULL, AV_LOG_INFO, "Bitrate: %lld bps\n", fmt_ctx->bit_rate);
    av_log(NULL, AV_LOG_INFO, "Number of streams: %d\n", fmt_ctx->nb_streams);

    // 打印元数据
    AVDictionaryEntry *tag = NULL;
    av_log(NULL, AV_LOG_INFO, "\n===== Metadata =====\n");
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_INFO, "%s: %s\n", tag->key, tag->value);
    }

    // 打印流信息
    av_log(NULL, AV_LOG_INFO, "\n===== Streams Information =====\n");
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++) {
        AVStream *stream = fmt_ctx->streams[i];
        av_log(NULL, AV_LOG_INFO, "Stream #%d:\n", i);
        av_log(NULL, AV_LOG_INFO, "  Type: %s\n", 
            av_get_media_type_string(stream->codecpar->codec_type));
        av_log(NULL, AV_LOG_INFO, "  Codec: %s\n", 
            avcodec_get_name(stream->codecpar->codec_id));
        av_log(NULL, AV_LOG_INFO, "  Timebase: %d/%d\n", 
            stream->time_base.num, stream->time_base.den);
        av_log(NULL, AV_LOG_INFO, "  Duration: %.2f seconds\n", 
            stream->duration * av_q2d(stream->time_base));
    }

    // 关闭音视频文件
    avformat_close_input(&fmt_ctx);
    return 0;
}