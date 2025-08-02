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
    template<typename... Args> void info(int category, const char *fmt, Args... args);
    template<typename... Args> void debug(int category, const char *fmt, Args... args);
    template<typename... Args> void warn(int category, const char *fmt, Args... args);
    template<typename... Args> void error(int category, const char *fmt, Args... args);
    template<typename... Args> void log(const std::string &fmt, Args... args);
    template<typename... Args> void info(int category, const std::string &fmt, Args... args);
    template<typename... Args> void debug(int category, const std::string &fmt, Args... args);
    template<typename... Args> void warn(int category, const std::string &fmt, Args... args);
    template<typename... Args> void error(int category, const std::string &fmt, Args... args);

    class logger_write_error : public std::exception {
    private:
        std::string error;
    public:
        logger_write_error(const char *message) : error(message) {}
        logger_write_error(const std::string &message) : error(message) {}
        const char *what() const noexcept override {
            return error.c_str();
        }
        virtual ~logger_write_error() = default;
    };
}

#include <logger.ipp>