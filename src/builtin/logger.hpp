#pragma once

#include <cstdarg>
#include <cstdint>

namespace Soundhouse::Logging
{
#ifndef LOGGER_LEVEL_LIST
#define LOGGER_LEVEL_LIST(M)                                                                                                                                                                           \
    M(All)                                                                                                                                                                                             \
    M(Trace)                                                                                                                                                                                           \
    M(Debug)                                                                                                                                                                                           \
    M(Info)                                                                                                                                                                                            \
    M(Warn)                                                                                                                                                                                            \
    M(Error)                                                                                                                                                                                           \
    M(Critical)
#endif

    /**
     * @brief A enum which contains the severity range between "Trace" to "Critical"
     *
     */
    enum class LoggerLevel : uint8_t
    {
#define LOGGER_LEVEL_ENUM_ENTRY(name) name,
        LOGGER_LEVEL_LIST(LOGGER_LEVEL_ENUM_ENTRY)
#undef LOGGER_LEVEL_ENUM_ENTRY
    };

    /**
     * @brief Time-based resolution for logging
     *
     */
    enum class LoggerTimeResolution
    {
        Seconds,
        Milliseconds,
        Microseconds,
        Nanoseconds
    };

    /**
     * @brief Ripped directly from HawkTuahSystem and stripped down to be Windows/Linux compatible. And, yeah. I'm not supporting MacOS for this.
     *
     */
    class Logger
    {
        public:
            explicit Logger(const char *name, LoggerLevel level = LoggerLevel::Info, LoggerTimeResolution resolution = LoggerTimeResolution::Microseconds);

            void set_level(LoggerLevel level);

            void log(LoggerLevel level, const char *fmt, ...);

            void trace(const char *fmt, ...);
            void debug(const char *fmt, ...);
            void info(const char *fmt, ...);
            void warn(const char *fmt, ...);
            void error(const char *fmt, ...);
            void critical(const char *fmt, ...);

        private:
            void vlog(LoggerLevel level, const char *fmt, va_list ap);
            void platform_write(LoggerLevel level, uint64_t timestamp, const char *message);

            const char *label_for(LoggerLevel level);
            const char *color_for(LoggerLevel level);
            const char *cast_to_readable_format(uint64_t timestamp);

            uint64_t    platform_time();

            void        format_timestamp(uint64_t ts, LoggerTimeResolution res, char *buffer, std::size_t sz);

        private:
            const char          *m_name;
            LoggerLevel          m_level;
            LoggerTimeResolution m_resolution;

            static constexpr std::size_t LOG_BUFFER_SIZE = 1028;
    };
} // namespace Soundhouse::Logging