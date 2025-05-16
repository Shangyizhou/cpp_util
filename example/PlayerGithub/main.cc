#include "Media/event_loop.h"
#include "Media/player.h"

int main(int argc, char *argv[]) {
    //if (argc < 2) {
    //    spdlog::info("usage: {} input", argv[0]);
    //    return -1;
    //}

    Player player;
    player.open("../video/BigBuckBunny_320x180.mp4");
    player.start();

    EventLoop loop;
    return loop.run(player);
}