#include "Core/Logger.hh"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace DWE {
    Logger::Logger(std::string name)
    {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _real_logger = std::make_shared<spdlog::logger>(name, sink);
    }

    Logger::Logger(std::string name, spdlog::sinks_init_list sinks)
    {
        _real_logger = std::make_shared<spdlog::logger>("custom_logger", sinks);
    }

    void Logger::setPattern(const std::string &pattern)
    {
        _real_logger->set_pattern(pattern);
    }

    void Logger::setLevel(LogLevel log_level)
    {
        switch(log_level) {
            case LogLevel::trace: _real_logger->set_level(spdlog::level::trace); break;
            case LogLevel::info: _real_logger->set_level(spdlog::level::info); break;
            case LogLevel::warn: _real_logger->set_level(spdlog::level::warn); break;
            case LogLevel::debug: _real_logger->set_level(spdlog::level::debug); break;
            case LogLevel::err: _real_logger->set_level(spdlog::level::err); break;
            case LogLevel::critical: _real_logger->set_level(spdlog::level::critical); break;
            case LogLevel::off: _real_logger->set_level(spdlog::level::off); break;
            case LogLevel::n_levels: _real_logger->set_level(spdlog::level::n_levels); break;
            default: break;
        }
    }

    void Logger::flushOn(LogLevel log_level) 
    {
        switch(log_level) {
            case LogLevel::trace: _real_logger->flush_on(spdlog::level::trace); break;
            case LogLevel::info: _real_logger->flush_on(spdlog::level::info); break;
            case LogLevel::warn: _real_logger->flush_on(spdlog::level::warn); break;
            case LogLevel::debug: _real_logger->flush_on(spdlog::level::debug); break;
            case LogLevel::err: _real_logger->flush_on(spdlog::level::err); break;
            case LogLevel::critical: _real_logger->flush_on(spdlog::level::critical); break;
            case LogLevel::off: _real_logger->flush_on(spdlog::level::off); break;
            case LogLevel::n_levels: _real_logger->flush_on(spdlog::level::n_levels); break;
            default: break;
        }
    }

    void Logger::log(LogLevel level, const char* msg)
    {
        switch(level) {
        case LogLevel::trace: trace(msg); break;
        case LogLevel::info: info(msg); break;
        case LogLevel::warn: warn(msg); break;
        case LogLevel::debug: debug(msg); break;
        case LogLevel::err: err(msg); break;
        case LogLevel::critical: critical(msg); break;
        default: break;
        }
    }

    void Logger::trace(const char *msg) { _real_logger->trace(msg); }
    void Logger::info(const char *msg) { _real_logger->info(msg); }
    void Logger::warn(const char *msg) { _real_logger->warn(msg); }
    void Logger::debug(const char *msg) { _real_logger->debug(msg); }
    void Logger::err(const char *msg) { _real_logger->error(msg); }
    void Logger::critical(const char *msg) { _real_logger->critical(msg); }
}
