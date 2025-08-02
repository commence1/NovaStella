#pragma once

#include <SDL3/SDL_log.h>

#include <exception>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include <fmt/format.h>

namespace logger {
    template<typename... Args> void log(bool write, const char *fmt, Args... args);
    template<typename... Args> void info(bool write, int category, const char *fmt, Args... args);
    template<typename... Args> void debug(bool write, int category, const char *fmt, Args... args);
    template<typename... Args> void warn(bool write, int category, const char *fmt, Args... args);
    template<typename... Args> void error(bool write, int category, const char *fmt, Args... args);

    template<typename... Args> void log(bool write, const std::string &fmt, Args... args);
    template<typename... Args> void info(bool write, int category, const std::string &fmt, Args... args);  
    template<typename... Args> void debug(bool write, int category, const std::string &fmt, Args... args);
    template<typename... Args> void warn(bool write, int category, const std::string &fmt, Args... args);
    template<typename... Args> void error(bool write, int category, const std::string &fmt, Args... args);

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