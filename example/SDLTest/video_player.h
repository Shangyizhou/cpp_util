// video_player.hpp
#pragma once

extern "C" {
#include <SDL2/SDL.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}
#include <string>
#include <memory>

class VideoPlayer {
public:
    explicit VideoPlayer(const std::string& filepath);
    ~VideoPlayer();

    bool initialize();
    void play();
    void stop();

private:
    struct AVFrameDeleter {
        void operator()(AVFrame* frame) const { av_frame_free(&frame); }
    };
    struct SwsContextDeleter {
        void operator()(SwsContext* ctx) const { sws_freeContext(ctx); }
    };
    struct AVCodecContextDeleter {
        void operator()(AVCodecContext* ctx) const { avcodec_free_context(&ctx); }
    };
    struct AVFormatContextDeleter {
        void operator()(AVFormatContext* ctx) const { avformat_close_input(&ctx); }
    };

    bool init_sdl();
    bool init_ffmpeg();
    bool init_video_stream();
    bool init_audio_stream();
    void process_events();
    void render_frame(AVFrame* frame);

    std::string filepath_;
    bool is_playing_;

    // SDL相关资源
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    // FFmpeg相关资源
    std::unique_ptr<AVFormatContext, AVFormatContextDeleter> format_ctx_;
    std::unique_ptr<AVCodecContext, AVCodecContextDeleter> video_codec_ctx_;
    std::unique_ptr<AVCodecContext, AVCodecContextDeleter> audio_codec_ctx_;
    std::unique_ptr<AVFrame, AVFrameDeleter> frame_;
    std::unique_ptr<SwsContext, SwsContextDeleter> sws_ctx_;
    
    int video_stream_index_;
    int audio_stream_index_;
};
