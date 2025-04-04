#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace tool
{
    class GlobalLogger
    {
    public:
        static std::shared_ptr<spdlog::logger> getInstance();

    private:
        static void Init();

        GlobalLogger() = delete;
        GlobalLogger(const GlobalLogger &) = delete;
        GlobalLogger(GlobalLogger &&) = delete;
        GlobalLogger &operator=(const GlobalLogger &) = delete;
        GlobalLogger &operator=(GlobalLogger &&) = delete;

        static std::shared_ptr<spdlog::logger> logger_;
        static std::once_flag initInstanceFlag;
    };

    #define LOG_BASE(level, ...) tool::GlobalLogger::getInstance()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)
    #define LOG_INFO(...)  LOG_BASE(spdlog::level::info, __VA_ARGS__)
    #define LOG_DEBUG(...) LOG_BASE(spdlog::level::debug, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_BASE(spdlog::level::err, __VA_ARGS__)
    #define LOG_WARN(...)  LOG_BASE(spdlog::level::warn, __VA_ARGS__)

} // namespace tool

