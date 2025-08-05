#pragma once

#include <logger.hpp>

namespace {
    void write_to_file(const std::filesystem::path &filePath, const std::string& message) {
        std::filesystem::create_directory("../logs");
        std::ofstream file(filePath, std::ios::out | std::ios::app);
        if (!file) {
            throw logger::logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
        }
        file << message << '\n';
        if (!file) {
            throw logger::logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
        }
    }
}

template<typename... Args> void logger::log(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "log.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::info(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "info.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::debug(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "debug.log";
    write_to_file(filePath, message);
}

template<typename... Args> void logger::warn(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "warn.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::error(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "error.log";
    write_to_file(filePath, message);
}

template<typename... Args> void logger::log(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "log.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::info(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "info.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::debug(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "debug.log";
    write_to_file(filePath, message);
}

template<typename... Args> void logger::warn(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "warn.log";
    write_to_file(filePath, message);
}
template<typename... Args> void logger::error(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    const auto filePath = std::filesystem::path("../logs") / "error.log";
    write_to_file(filePath, message);
}