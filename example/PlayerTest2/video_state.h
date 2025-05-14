#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <SDL2/SDL.h>

#include <libavutil/avutil.h>
#include <libavutil/fifo.h>
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
};
#endif

struct VideoParam
{
    // for video
    int video_index;
    AVStream *video_st;
    AVCodecContext *video_ctx;
    PacketQueue videoq;
    AVPacket video_pkt;
    struct SwsContext *sws_ctx;
};

struct AudioParam
{
    // for audio
    int audio_index;
    AVStream *audio_st;
    AVCodecContext *audio_ctx;
    PacketQueue audioq;
    uint8_t *audio_buf;
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;
    AVFrame audio_frame;
    AVPacket audio_pkt;
    uint8_t *audio_pkt_data;
    int audio_pkt_size;
    struct SwrContext *audio_swr_ctx;
};

struct SDLParam
{
    int width;
    int height;
    int xleft;
    int ytop;
    SDL_Texture *texture;
    SDL_Thread *read_tid;
    SDL_Thread *decode_tid;

    int quit;
};

struct ClockParam {
    //sync
    int             av_sync_type;

    double          audio_clock; ///< the time of have audio frame
    double          frame_timer; ///< the time of have played video frame 
    double          frame_last_pts;
    double          frame_last_delay;

    double          video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
    double          video_current_pts; ///<current displayed pts (different from video_clock if frame fifos are used)
    int64_t         video_current_pts_time;  ///< sys time (av_gettime) at which we updated video_current_pts - used to have running video pts
};

struct VideoState
{
    // for multi-media file
    char *filename;
    AVFormatContext *ic;

    FrameQueue pictq;

    int quit;

    VideoParam video_param;
    AudioParam audio_param;
    SDLParam sdl_param;
    ClockParam clock_param;

    
    void video_display(VideoState *is);
};


class Player {
public:
    Player() = default;
    ~Player();

    void Init();

    void Open(const std::string file_name);
    
    void Loop();

private:
    void InitSDL();
    VideoState video_state;
    std::string file_name;
};

void Player::Init() {
    // Init SDL
    SDL_setenv("SDL_AUDIODRIVER", "alsa", 1);

    int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if(SDL_Init(flags)) {
        av_log(NULL, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    //creat window from SDL
    SDL_Renderer *renderer;
    SDL_Window *win = SDL_CreateWindow("Media Player",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
                default_width, default_height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(win) {
        renderer = SDL_CreateRenderer(win, -1, 0);
    }

    if(!win || !renderer){
        av_log(NULL, AV_LOG_FATAL, "Failed to create window or renderer!\n");
        do_exit(NULL);
    }
}

void Player::Open() {
    // stream_open file
    // set video_state
}

void Player::Loop() {
    SDL_Event       event;
    for(;;) {
        SDL_WaitEvent(&event);
        switch(event.type) {
        case FF_QUIT_EVENT:
        case SDL_QUIT:
            is->quit = 1;
            do_exit(is);
            break;
        case FF_REFRESH_EVENT:
            video_refresh_timer(event.user.data1);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    Player player;
    player.Init();
    player.Open(argv[1]);
    player.Loop();

    return 0;
}
