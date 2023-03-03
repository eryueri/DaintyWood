#pragma once

#include <spdlog/spdlog.h>

namespace DWE {
    enum class LogLevel {
        trace, info, warn, debug, err, critical, off, n_levels
    };
    class Logger {
    public:
        Logger() = delete; // all logger should have name
        Logger(std::string name);
        Logger(std::string name, spdlog::sinks_init_list sinks);
        ~Logger() = default;

        void setPattern(const std::string& pattern);
        void setLevel(LogLevel log_level);
        void flushOn(LogLevel log_level);

        void log(LogLevel level, const char* msg);

        void info(const char* msg);
        void warn(const char* msg);
        void debug(const char* msg);
        void trace(const char* msg);
        void err(const char* msg);
        void critical(const char* msg);
        void off(const char* msg);

    private:
        std::shared_ptr<spdlog::logger> _real_logger = nullptr;
    };
}
