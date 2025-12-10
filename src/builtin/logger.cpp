#include <chrono>
#include <cstdint>
#include <cstdio>
#include <inttypes.h>

#include "logger.hpp"

namespace Soundhouse::Logging
{
    Logger::Logger(const char *name, LoggerLevel level, LoggerTimeResolution resolution) : m_name(name), m_level(level), m_resolution(resolution)
    {
    }

    void Logger::set_level(LoggerLevel loggerLevel)
    {
        m_level = loggerLevel;
    }

    void Logger::log(LoggerLevel loggerLevel, const char *fmt, ...)
    {
        if (loggerLevel < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(loggerLevel, fmt, args);
        va_end(args);
    }

    void Logger::trace(const char *fmt, ...)
    {
        if (LoggerLevel::Trace < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Trace, fmt, args);
        va_end(args);
    }

    void Logger::debug(const char *fmt, ...)
    {
        if (LoggerLevel::Debug < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Debug, fmt, args);
        va_end(args);
    }

    void Logger::info(const char *fmt, ...)
    {
        if (LoggerLevel::Info < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Info, fmt, args);
        va_end(args);
    }

    void Logger::warn(const char *fmt, ...)
    {
        if (LoggerLevel::Warn < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Warn, fmt, args);
        va_end(args);
    }

    void Logger::error(const char *fmt, ...)
    {
        if (LoggerLevel::Error < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Error, fmt, args);
        va_end(args);
    }

    void Logger::critical(const char *fmt, ...)
    {
        if (LoggerLevel::Critical < m_level)
        {
            return;
        }

        va_list args;
        va_start(args, fmt);
        vlog(LoggerLevel::Critical, fmt, args);
        va_end(args);
    }

    uint64_t Logger::platform_time()
    {
        static const auto start_time = std::chrono::steady_clock::now();
        auto              now        = std::chrono::steady_clock::now();

        return std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();;
    }

    void Logger::vlog(LoggerLevel loggerLevel, const char *fmt, va_list ap)
    {
        char buffer[LOG_BUFFER_SIZE];
        vsnprintf(buffer, LOG_BUFFER_SIZE, fmt, ap);

        uint64_t ts = platform_time();
        platform_write(loggerLevel, ts, buffer);
    }

    const char *Logger::label_for(LoggerLevel level)
    {
        switch (level)
        {
            case LoggerLevel::Trace:
                return "TRACE";
            case LoggerLevel::Debug:
                return "DEBUG";
            case LoggerLevel::Info:
                return "INFO";
            case LoggerLevel::Warn:
                return "WARN";
            case LoggerLevel::Error:
                return "ERROR";
            case LoggerLevel::Critical:
                return "CRITICAL";
            default:
                return "UNKNOWN";
        }
    }

    const char *Logger::color_for(LoggerLevel level)
    {
        switch (level)
        {
            case LoggerLevel::Trace:
                return "\033[90m";
            case LoggerLevel::Debug:
                return "\033[36m";
            case LoggerLevel::Info:
                return "\033[32m";
            case LoggerLevel::Warn:
                return "\033[33m";
            case LoggerLevel::Error:
                return "\033[31m";
            case LoggerLevel::Critical:
                return "\033[41;97m";
            default:
                return "";
        }
    }

    const char *Logger::cast_to_readable_format(uint64_t timestamp)
    {
        static thread_local char buf[64];

        std::time_t sec = timestamp / 1'000'000'000ULL;
        std::tm     tm{};

        localtime_r(&sec, &tm);

        std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);

        return buf;
    }

    void Logger::platform_write(LoggerLevel level, uint64_t timestamp, const char *message)
    {
        char ts_buf[64];

        format_timestamp(timestamp, m_resolution, ts_buf, sizeof(ts_buf));

        const char *label = label_for(level);
        const char *color = color_for(level);

        std::printf("[%s] %s[%s]\033[0m (%s) %s\n", ts_buf, color, label, m_name, message);
    }

    void Logger::format_timestamp(uint64_t ts, LoggerTimeResolution resolution, char *buffer, size_t size)
    {
        switch (resolution)
        {
            case LoggerTimeResolution::Seconds:
                snprintf(buffer, size, "%.6f s", ts / 1e9);  // ns -> s
                break;

            case LoggerTimeResolution::Milliseconds:
                snprintf(buffer, size, "%.3f ms", ts / 1e6); // ns -> ms
                break;

            case LoggerTimeResolution::Microseconds:
                snprintf(buffer, size, "%.3f µs", ts / 1e3); // ns -> µs
                break;

            case LoggerTimeResolution::Nanoseconds:
                snprintf(buffer, size, "%llu ns", (unsigned long long)ts);
                break;
        }
    }
} // namespace Soundhouse::Logging