#include <spdlog/spdlog.h>
#include "audio_player.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <libavutil/time.h>
#include <libavutil/opt.h>

#ifdef __cplusplus
}
#endif

void callback(void *opaque, Uint8 *steram, int len) {
    if (!opaque) {
        return;
    }
    AudioPlayer* player = static_cast<AudioPlayer*>(opaque);
    return player->run(steram, len);
}

AudioPlayer::AudioPlayer(std::shared_ptr<Context> ctx)
    : m_ctx(ctx) {

}

int AudioPlayer::open() {
    // 定义 SDL 音频规格结构体，desired 为期望的音频规格，optained 为实际获得的音频规格
    SDL_AudioSpec desired, optained;

    // 设置期望的音频通道数（从音频解码器上下文中获取）
    desired.channels = m_ctx->audio_codec_ctx->ch_layout.nb_channels;

    // 设置期望的音频采样率（从音频解码器上下文中获取）
    desired.freq = m_ctx->audio_codec_ctx->sample_rate;

    // 设置音频格式为系统字节序的 16 位有符号整数
    desired.format = AUDIO_S16SYS;

    // 设置静音值为 0（静音时输出的音频数据）
    desired.silence = 0;

    // 设置音频缓冲区大小（以样本为单位），根据采样率和回调频率计算
    desired.samples = std::max(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(desired.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));

    // 设置音频回调函数，用于填充音频数据
    desired.callback = callback;

    // 设置回调函数的用户数据为当前对象
    desired.userdata = this;

    // 打开音频设备，使用期望的音频规格，允许频率和通道数的变化
    m_audio_dev_id = SDL_OpenAudioDevice(nullptr, 0, &desired, &optained, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);

    // 检查音频设备是否打开成功
    if (0 == m_audio_dev_id) {
        // 如果失败，记录错误日志
        spdlog::error("SDL_OpenAudio failed {}", SDL_GetError());
    }

    // TODO: 检查 optained 是否与 desired 匹配，并处理不支持的音频格式
    // m_audio_hw_buf_size = optained.size;

    // 设置音频硬件缓冲区大小（暂时硬编码为 2048）
    m_audio_hw_buf_size = 2048;

    // 计算每秒钟的音频数据字节数
    m_bytes_per_sec = av_samples_get_buffer_size(nullptr, desired.channels, desired.freq, AV_SAMPLE_FMT_S16, 1);

    // 返回音频设备 ID，如果失败则为 0
    return m_audio_dev_id;
}

int AudioPlayer::start() {
    SDL_PauseAudioDevice(m_audio_dev_id, 0);
    return 0;
}

int AudioPlayer::stop() {
    SDL_PauseAudioDevice(m_audio_dev_id, 1);
    return 0;
}

int AudioPlayer::close() {
    stop();
    return 0;
}

void AudioPlayer::update_volume(int volume) {
    int temp = m_volume;
    temp += volume * av_q2d(AVRational{ 128, 100 });
    if (temp > MAX_VOLUME_VALUE) {
        temp = MAX_VOLUME_VALUE;
    }
    else if (temp < MIN_VOLUME_VALUE) {
        temp = MIN_VOLUME_VALUE;
    }
    m_volume = temp;
    spdlog::info("volume={}", temp);
}

void AudioPlayer::toggle_mute() {
    m_muted = !m_muted;
}

bool AudioPlayer::is_muted() const {
    return m_muted;
}

void AudioPlayer::run(Uint8* stream, int len) {
    int len1 = 0;
    int64_t audio_callback_time = av_gettime_relative();
    while (len > 0) {
        // audio buf缓存已经读完
        if (m_current_audio_buf_index >= m_current_audio_buf_size) {
            if (get_audio_data() < 0) {
                m_current_audio_buf_data = nullptr;
                m_current_audio_buf_size = SDL_AUDIO_MIN_BUFFER_SIZE;
            }
            // TODO is->show_mode != SHOW_MODE_VIDEO
            // 设置buf大小以及读写位置
            // m_current_audio_buf_size = data.length;
            // m_current_audio_buf_index = 0;
        }
        // 复制音频数据到SDL的内存
        len1 = m_current_audio_buf_size - m_current_audio_buf_index;
        if (len1 > len) {
            len1 = len;
        }
        if (!m_muted && m_current_audio_buf_data && SDL_MIX_MAXVOLUME == m_volume) {
            memcpy(stream, (uint8_t*)m_current_audio_buf_data + m_current_audio_buf_index, len1);
        }
        else {
            memset(stream, 0, len1);
            if (!m_muted && m_current_audio_buf_data) {
                SDL_MixAudioFormat(stream, m_current_audio_buf_data + m_current_audio_buf_index, AUDIO_S16SYS, len1, m_volume);
            }
        }
        len -= len1;
        stream += len1;
        m_current_audio_buf_index += len1;
    }

    int audio_write_buf_size = m_current_audio_buf_size - m_current_audio_buf_index;
    if (!isnan(m_current_audio_clock)) {
        // spdlog::info("pts={:.4f}, serial={}, time={:.4f}", m_current_audio_clock - (double)(2 * m_audio_hw_buf_size + audio_write_buf_size) / m_bytes_per_sec, m_current_audio_clock_serial, audio_callback_time / 1000000.0);
        m_ctx->audio_clock.set_at(m_current_audio_clock - (double)(2 * m_audio_hw_buf_size + audio_write_buf_size) / m_bytes_per_sec, m_current_audio_clock_serial, audio_callback_time / 1000000.0);
    }
    return;
}

int AudioPlayer::get_audio_data() {
    m_current_audio_buf_index = 0; // 重置当前音频缓冲区的读取索引

    if (m_ctx->paused) { // 如果播放器处于暂停状态
        return -1; // 返回 -1 表示无法获取音频数据
    }

    Frame *af = nullptr; // 定义一个指向音频帧的指针

    do {
        af = m_ctx->audio_frame_queue.peek_readable(); // 从音频帧队列中获取一个可读的帧
        if (!af) { // 如果没有可读的帧
            return -1; // 返回 -1 表示无法获取音频数据
        }
        m_ctx->audio_frame_queue.next(); // 移动到队列中的下一帧
    } while (af->serial != m_ctx->audio_packet_queue.serial()); // 确保帧的序列号与音频包队列的序列号匹配

    if (!m_ctx->audio_swr_ctx) { // 如果音频重采样上下文尚未初始化
        m_ctx->audio_swr_ctx = swr_alloc(); // 分配一个新的音频重采样上下文
        if (!m_ctx->audio_swr_ctx) { // 如果分配失败
            spdlog::error("swr_alloc failed"); // 记录错误日志
            return -1; // 返回 -1 表示失败
        }

        // 设置重采样上下文的输入参数
        av_opt_set_chlayout(m_ctx->audio_swr_ctx, "in_chlayout", &af->frame->ch_layout, 0);
        av_opt_set_int(m_ctx->audio_swr_ctx, "in_sample_rate", af->frame->sample_rate, 0);
        av_opt_set_sample_fmt(m_ctx->audio_swr_ctx, "in_sample_fmt", static_cast<AVSampleFormat>(af->frame->format), 0);

        // 设置重采样上下文的输出参数
        av_opt_set_chlayout(m_ctx->audio_swr_ctx, "out_chlayout", &af->frame->ch_layout, 0);
        av_opt_set_int(m_ctx->audio_swr_ctx, "out_sample_rate", af->frame->sample_rate, 0);
        av_opt_set_sample_fmt(m_ctx->audio_swr_ctx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

        if (swr_init(m_ctx->audio_swr_ctx) < 0) { // 初始化音频重采样上下文
            spdlog::error("swr_init failed"); // 记录错误日志
            return -1; // 返回 -1 表示失败
        }
    }

    if (m_ctx->audio_swr_ctx) { // 如果音频重采样上下文已初始化
        const uint8_t **in = (const uint8_t **)af->frame->extended_data; // 获取输入音频数据
        uint8_t **out = &m_current_audio_buf_data; // 指向输出音频缓冲区
        int data_size = av_samples_get_buffer_size(
            nullptr,
            af->frame->ch_layout.nb_channels,
            af->frame->nb_samples,
            AV_SAMPLE_FMT_S16,
            0); // 计算输出音频数据的大小

        av_fast_malloc(&m_current_audio_buf_data, (unsigned int *)&m_current_audio_buf_size, data_size); // 分配输出音频缓冲区
        if (!m_current_audio_buf_data) { // 如果分配失败
            spdlog::error("av_fast_malloc failed"); // 记录错误日志
            return -1; // 返回 -1 表示失败
        }

        int len = swr_convert(
            m_ctx->audio_swr_ctx,
            out,
            af->frame->nb_samples,
            in,
            af->frame->nb_samples); // 执行音频重采样
        if (len < 0) { // 如果重采样失败
            spdlog::error("swr_convert failed"); // 记录错误日志
            return -1; // 返回 -1 表示失败
        }

        m_current_audio_buf_size = av_samples_get_buffer_size(
            nullptr,
            af->frame->ch_layout.nb_channels,
            len,
            AV_SAMPLE_FMT_S16,
            1); // 更新输出音频缓冲区的大小
    } else { // 如果不需要重采样
        int data_size = av_samples_get_buffer_size(
            nullptr,
            af->frame->ch_layout.nb_channels,
            af->frame->nb_samples,
            static_cast<AVSampleFormat>(af->frame->format),
            1); // 计算音频数据的大小
        m_current_audio_buf_data = af->frame->data[0]; // 指向音频帧的数据
        m_current_audio_buf_size = data_size; // 设置音频缓冲区的大小
    }

    // TODO synchronize_audio
    // TODO af->frame->format != audio_src.fmt
    // TODO swr_ctx

    double audio_clock0 = m_current_audio_clock; // 保存当前音频时钟
    if (!isnan(af->pts)) { // 如果帧的 PTS 有效
        m_current_audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate; // 更新音频时钟
    } else {
        m_current_audio_clock = NAN; // 如果 PTS 无效，将音频时钟设置为 NAN
    }
    m_current_audio_clock_serial = af->serial; // 更新音频时钟的序列号
    m_last_audio_clock = m_current_audio_clock; // 保存最后的音频时钟

    return 0; // 返回 0 表示成功
}