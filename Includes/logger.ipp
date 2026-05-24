#pragma once

#include <logger.hpp>

namespace logger {
    namespace detail {
        inline void write_log(const std::string& message) {
            const auto filePath = std::filesystem::path("logs") / "log.log";
            std::filesystem::create_directories("logs");
            std::ofstream file(filePath, std::ios::out | std::ios::app);
            if (!file) {
                throw logger_write_error("Failed to open file: \"" + filePath.string() + "\"");
            }
            file << message << '\n';
            if (!file) {
                throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
            }
        }

        template<typename... Args>
        std::string format_message(const char* level, const char* fmt, Args&&... args) {
            auto content = fmt::format(fmt, std::forward<Args>(args)...);
            return fmt::format("[{}] [{}] {}", level, SDL_GetTicksNS() / 1000000, content);
        }
    }

    template<typename... Args>
    void log(const char* fmt, Args... args) {
        detail::write_log(detail::format_message("LOG", fmt, args...));
    }

    template<typename... Args>
    void info(const char* fmt, Args... args) {
        detail::write_log(detail::format_message("INFO", fmt, args...));
    }

    template<typename... Args>
    void debug(const char* fmt, Args... args) {
        detail::write_log(detail::format_message("DEBUG", fmt, args...));
    }

    template<typename... Args>
    void warn(const char* fmt, Args... args) {
        detail::write_log(detail::format_message("WARN", fmt, args...));
    }

    template<typename... Args>
    void error(const char* fmt, Args... args) {
        detail::write_log(detail::format_message("ERROR", fmt, args...));
    }

    template<typename... Args>
    void log(const std::string& fmt, Args... args) {
        detail::write_log(detail::format_message("LOG", fmt.c_str(), args...));
    }

    template<typename... Args>
    void info(const std::string& fmt, Args... args) {
        detail::write_log(detail::format_message("INFO", fmt.c_str(), args...));
    }

    template<typename... Args>
    void debug(const std::string& fmt, Args... args) {
        detail::write_log(detail::format_message("DEBUG", fmt.c_str(), args...));
    }

    template<typename... Args>
    void warn(const std::string& fmt, Args... args) {
        detail::write_log(detail::format_message("WARN", fmt.c_str(), args...));
    }

    template<typename... Args>
    void error(const std::string& fmt, Args... args) {
        detail::write_log(detail::format_message("ERROR", fmt.c_str(), args...));
    }
}
