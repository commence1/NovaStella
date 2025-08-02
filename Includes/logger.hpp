#pragma once

#include <SDL3/SDL_log.h>

#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include <fmt/format.h>

namespace logger {
    template<typename... Args> void log(const char *fmt, Args... args);
    template<typename... Args> void info(const char *fmt, Args... args);
    template<typename... Args> void debug(const char *fmt, Args... args);
    template<typename... Args> void warn(const char *fmt, Args... args);
    template<typename... Args> void error(const char *fmt, Args... args);

    template<typename... Args> void log(const std::string &fmt, Args... args);
    template<typename... Args> void info(const std::string &fmt, Args... args);  
    template<typename... Args> void debug(const std::string &fmt, Args... args);
    template<typename... Args> void warn(const std::string &fmt, Args... args);
    template<typename... Args> void error(const std::string &fmt, Args... args);

    class logger_write_error : public std::exception {
    private:
        std::string msg;
    public:
        explicit logger_write_error(const char *message) : msg(message) {}
        explicit logger_write_error(const std::string &message) : msg(message) {}
        const char *what() const noexcept override {
            return msg.c_str();
        }
    };
}

#include <logger.ipp>