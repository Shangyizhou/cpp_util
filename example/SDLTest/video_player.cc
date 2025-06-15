// video_player.cpp
#include "video_player.h"
#include <stdexcept>

VideoPlayer::VideoPlayer(const std::string& filepath) 
    : filepath_(filepath), is_playing_(false),
      window_(nullptr), renderer_(nullptr), texture_(nullptr),
      video_stream_index_(-1), audio_stream_index_(-1) {}

VideoPlayer::~VideoPlayer() {
    stop();
}

bool VideoPlayer::initialize() {
    if (!init_sdl()) {
        return false;
    }
    if (!init_ffmpeg()) {
        return false;
    }
    return true;
}

bool VideoPlayer::init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("FFmpeg Player", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED,
                              640, 480, 
                              SDL_WINDOW_RESIZABLE);
    if (!window_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool VideoPlayer::init_ffmpeg() {
    AVFormatContext* fmt_ctx = nullptr;
    if (avformat_open_input(&fmt_ctx, filepath_.c_str(), nullptr, nullptr) < 0) {
        SDL_Log("Could not open video file");
        return false;
    }
    format_ctx_.reset(fmt_ctx);

    if (avformat_find_stream_info(format_ctx_.get(), nullptr) < 0) {
        SDL_Log("Could not find stream information");
        return false;
    }

    if (!init_video_stream()) {
        return false;
    }

    // 可以在这里添加音频流初始化
    // if (!init_audio_stream()) {
    //     return false;
    // }

    frame_.reset(av_frame_alloc());
    if (!frame_) {
        SDL_Log("Could not allocate frame");
        return false;
    }

    return true;
}

bool VideoPlayer::init_video_stream() {
    video_stream_index_ = av_find_best_stream(format_ctx_.get(), AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (video_stream_index_ < 0) {
        SDL_Log("Could not find video stream");
        return false;
    }

    AVStream* video_stream = format_ctx_->streams[video_stream_index_];
    const AVCodec* decoder = avcodec_find_decoder(video_stream->codecpar->codec_id);
    if (!decoder) {
        SDL_Log("Unsupported codec");
        return false;
    }

    AVCodecContext* codec_ctx = avcodec_alloc_context3(decoder);
    if (!codec_ctx) {
        SDL_Log("Could not allocate codec context");
        return false;
    }

    if (avcodec_parameters_to_context(codec_ctx, video_stream->codecpar) < 0) {
        SDL_Log("Could not copy codec parameters");
        avcodec_free_context(&codec_ctx);
        return false;
    }

    if (avcodec_open2(codec_ctx, decoder, nullptr) < 0) {
        SDL_Log("Could not open codec");
        avcodec_free_context(&codec_ctx);
        return false;
    }

    video_codec_ctx_.reset(codec_ctx);

    // 创建SDL纹理
    texture_ = SDL_CreateTexture(
        renderer_,
        SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING,
        video_codec_ctx_->width,
        video_codec_ctx_->height
    );
    
    if (!texture_) {
        SDL_Log("Could not create texture: %s", SDL_GetError());
        return false;
    }

    // 初始化图像转换器
    sws_ctx_.reset(sws_getContext(
        video_codec_ctx_->width, video_codec_ctx_->height, video_codec_ctx_->pix_fmt,
        video_codec_ctx_->width, video_codec_ctx_->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    ));
    if (!sws_ctx_) {
        SDL_Log("Could not create sws context");
        return false;
    }

    return true;
}

void VideoPlayer::play() {
    if (is_playing_) return;
    is_playing_ = true;

    AVPacket packet;
    while (is_playing_) {
        process_events();

        if (av_read_frame(format_ctx_.get(), &packet) < 0) {
            break;
        }

        if (packet.stream_index == video_stream_index_) {
            if (avcodec_send_packet(video_codec_ctx_.get(), &packet) < 0) {
                SDL_Log("Error sending packet for decoding");
                continue;
            }

            while (avcodec_receive_frame(video_codec_ctx_.get(), frame_.get()) == 0) {
                render_frame(frame_.get());
                // 简单帧率控制
                SDL_Delay(1000 / 30);
            }
        }

        av_packet_unref(&packet);
    }
}

void VideoPlayer::stop() {
    is_playing_ = false;
    
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    
    SDL_Quit();
}

void VideoPlayer::process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                stop();
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    stop();
                }
                break;
        }
    }
}

void VideoPlayer::render_frame(AVFrame* frame) {
    SDL_UpdateYUVTexture(
        texture_,
        nullptr,
        frame->data[0], frame->linesize[0],
        frame->data[1], frame->linesize[1],
        frame->data[2], frame->linesize[2]
    );

    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}
