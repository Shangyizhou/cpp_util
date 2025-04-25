#include "logger.h"

namespace tool
{
    std::shared_ptr<spdlog::logger> GlobalLogger::logger_ = nullptr;
    std::once_flag GlobalLogger::initInstanceFlag;

    std::shared_ptr<spdlog::logger> GlobalLogger::getInstance()
    {
        std::call_once(initInstanceFlag, &GlobalLogger::Init);
        return logger_;
    }

    void GlobalLogger::Init()
    {
        spdlog::init_thread_pool(1 << 16, 4);
        logger_ = spdlog::stdout_color_mt("console");
        logger_->set_level(spdlog::level::info);
        logger_->set_pattern(
            "[%Y-%m-%d %H:%M:%S.%e] "  // 时间戳
            "[%^%-L%$] "               // 左对齐的字符级别（带颜色）
            "[thr %t] "                // 线程信息（固定宽度）
            "[%s:%#] "                 // 源文件:行号（去函数名）
            "%v"                       // 日志消息
        );
    }
}