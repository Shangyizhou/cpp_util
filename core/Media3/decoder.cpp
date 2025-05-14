#include "decoder.h"
#include <spdlog/spdlog.h>

Decoder::Decoder(std::shared_ptr<Context> ctx, AVMediaType media_type)
    : m_ctx(ctx),
      m_media_type(media_type),
      m_pkt(av_packet_alloc())
{
}

Decoder::~Decoder() {
    if (m_pkt) {
        av_packet_free(&m_pkt);
        m_pkt = nullptr;
    }
    close();
}

int Decoder::open() {
    const AVCodec *codec = nullptr;
    AVFormatContext *fmt_ctx = m_ctx->fmt_ctx;
    int stream_index = av_find_best_stream(fmt_ctx, m_media_type, -1, -1, &codec, 0);
    if (stream_index < 0 || stream_index >= (int)fmt_ctx->nb_streams) {
        spdlog::error("av_find_best_stream failed, stream_index={}", stream_index);
        return -1;
    }
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[stream_index]->codecpar);
    int ret = avcodec_open2(codec_ctx, codec, nullptr);
    if (ret < 0) {
        spdlog::error("avcodec_open2 failed, ret={}", ret);
        return -1;
    }
    return _open(stream_index, codec_ctx);
}

int Decoder::close() {
    m_queue->abort_request();
    m_ctx->audio_frame_queue.wakeup();
    m_ctx->video_frame_queue.wakeup();
    stop();
    return _close();
}

int Decoder::decode(AVCodecContext *codec_ctx, AVFrame *frame) {
    int ret = AVERROR(EAGAIN); // 初始化返回值为 AVERROR(EAGAIN)，表示需要更多数据

    for (;;) { // 无限循环，直到成功解码一帧或发生错误
        if (m_queue->serial() == m_pkt_serial) { // 检查当前队列的序列号是否与解码器的序列号匹配
            do {
                if (m_queue->request_aborted()) { // 如果解码请求被中止
                    return -1; // 返回 -1 表示解码失败
                }
                ret = avcodec_receive_frame(codec_ctx, frame); // 从解码器接收解码后的帧
                if (ret >= 0) { // 如果成功接收到帧
                    if (AVMEDIA_TYPE_AUDIO == codec_ctx->codec_type) { // 如果是音频流
                        AVRational tb = AVRational{1, frame->sample_rate}; // 定义时间基为 1/采样率
                        if (frame->pts != AV_NOPTS_VALUE) { // 如果帧的 PTS 有效
                            frame->pts = av_rescale_q(frame->pts, codec_ctx->pkt_timebase, tb); // 将 PTS 从包的时间基转换为音频时间基
                        } else if (m_next_pts != AV_NOPTS_VALUE) { // 如果下一帧的 PTS 有效
                            frame->pts = av_rescale_q(m_next_pts, m_next_pts_tb, tb); // 使用下一帧的 PTS
                        }
                        if (frame->pts != AV_NOPTS_VALUE) { // 如果帧的 PTS 仍然有效
                            m_next_pts = frame->pts + frame->nb_samples; // 更新下一帧的 PTS
                            m_next_pts_tb = tb; // 更新下一帧的时间基
                        }
                    } else if (AVMEDIA_TYPE_VIDEO == codec_ctx->codec_type) { // 如果是视频流
                        if (m_recoder_pts == -1) { // 如果记录的 PTS 为 -1
                            frame->pts = frame->best_effort_timestamp; // 使用最佳估计的时间戳
                        } else if (!m_recoder_pts) { // 如果记录的 PTS 为 0
                            frame->pts = frame->pkt_dts; // 使用包的 DTS（解码时间戳）
                        }
                    }
                }

                if (AVERROR_EOF == ret) { // 如果解码器到达文件末尾
                    m_finished = m_pkt_serial; // 标记解码完成
                    avcodec_flush_buffers(codec_ctx); // 刷新解码器缓冲区
                    return 0; // 返回 0 表示解码完成
                }

                if (ret >= 0) { // 如果成功解码一帧
                    return 1; // 返回 1 表示成功
                }
            } while (ret != AVERROR(EAGAIN)); // 如果需要更多数据，继续尝试接收帧
        }

        do {
            if (m_queue->count() == 0) { // 如果队列为空
                m_ctx->demux_cond.notify_one(); // 通知解复用器线程
            }
            if (m_packet_pending) { // 如果有未处理的包
                m_packet_pending = 0; // 重置标志
            } else {
                int old_serial = m_pkt_serial; // 保存当前序列号
                if (m_queue->get(m_pkt, 1, m_pkt_serial) < 0) { // 从队列中获取一个包
                    return -1; // 如果获取失败，返回 -1
                }
                if (old_serial != m_pkt_serial) { // 如果序列号发生变化
                    avcodec_flush_buffers(codec_ctx); // 刷新解码器缓冲区
                    m_finished = 0; // 重置完成标志
                    m_next_pts = m_start_pts; // 重置下一帧的 PTS
                    m_next_pts_tb = m_start_pts_tb; // 重置下一帧的时间基
                }
            }
            if (m_queue->serial() == m_pkt_serial) { // 如果队列的序列号与解码器的序列号匹配
                break; // 跳出循环
            }
            av_packet_unref(m_pkt); // 释放包的引用
        } while (1);

        // TODO subtitle
        // 处理字幕解码的逻辑（尚未实现）

        // TODO FrameData
        // 处理帧数据的逻辑（尚未实现）

        if (avcodec_send_packet(codec_ctx, m_pkt) == AVERROR(EAGAIN)) { // 将包发送到解码器
            m_packet_pending = 1; // 如果解码器需要更多数据，设置标志
        } else {
            av_packet_unref(m_pkt); // 释放包的引用
        }
    }
}

int Decoder::_open(int stream_index, AVCodecContext *codec_ctx) {
    return 0;
}

int Decoder::_close() {
    return 0;
}