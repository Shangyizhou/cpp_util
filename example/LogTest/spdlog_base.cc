#include <spdlog/spdlog.h>

int main() {
    // 设置全局日志级别
    spdlog::set_level(spdlog::level::debug);
    
    // 基本日志记录
    spdlog::trace("This is a trace message");
    spdlog::debug("This is a debug message");
    spdlog::info("This is an info message");
    spdlog::warn("This is a warning message");
    spdlog::error("This is an error message");
    spdlog::critical("This is a critical message");
    
    // 格式化日志
    spdlog::info("Welcome to {}!", "spdlog");
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("Easy padding in numbers like {:08d}", 12);
    
    return 0;
}