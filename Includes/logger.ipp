#pragma once

#include <logger.hpp>

template<typename... Args> void logger::log(bool write, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_Log("%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::info(bool write, int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogInfo(category, "%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::debug(bool write, int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogDebug(category, "%s", message.c_str());

    if (!write)
        return;

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

template<typename... Args> void logger::warn(bool write, int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogWarn(category, "%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::error(bool write, int category, const char *fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogError(category, "%s", message.c_str());

    if (!write)
        return;

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

template<typename... Args> void logger::log(bool write, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_Log("%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::info(bool write, int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogInfo(category, "%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::debug(bool write, int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogDebug(category, "%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::warn(bool write, int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogWarn(category, "%s", message.c_str());

    if (!write)
        return;

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
template<typename... Args> void logger::error(bool write, int category, const std::string &fmt, Args... args) {
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);

    SDL_LogError(category, "%s", message.c_str());

    if (!write)
        return;

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