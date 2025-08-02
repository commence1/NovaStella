#pragma once

#include <logger.hpp>

template<typename... Args> void logger::log(const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_Log("%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::info(int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogInfo(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::debug(int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogDebug(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::warn(int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogWarn(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::error(int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogError(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}

template<typename... Args> void logger::log(const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_Log("%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }    
}
template<typename... Args> void logger::info(int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogInfo(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::debug(int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogDebug(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::warn(int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogWarn(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}
template<typename... Args> void logger::error(int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogError(category, "%s", message.c_str());

    const auto filePath = std::filesystem::path("logs") / "log.log";
    std::filesystem::create_directory("logs");
    std::ofstream file(filePath, std::ios::out | std::ios::app);
    if (!file) {
        throw logger_write_error("Failed to open file: \"" + filePath.string() +"\"");
    }
    file << message << '\n';
    if (!file) {
        throw logger_write_error("Failed to write to file: \"" + filePath.string() + "\"");
    }
}