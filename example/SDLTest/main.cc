#include "video_player.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // 检查命令行参数，确保至少传入视频文件路径
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <video_file_path>" << std::endl;
        return -1;
    }
    
    std::string filepath = argv[1];
    // 创建 VideoPlayer 对象
    VideoPlayer player(filepath);
    
    // 初始化视频播放器
    if (!player.initialize()) {
        std::cerr << "Failed to initialize video player." << std::endl;
        return -1;
    }
    
    // 开始播放视频，内部循环处理事件和渲染
    player.play();
    
    return 0;
}