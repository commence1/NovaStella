#pragma once

#include <logger.hpp>

template<typename... Args> void log(const char *fmt, Args... args) {
    /*char buf[1024];
    snprintf(buf, sizeof(buf), fmt, std::forward<Args>(args)...);
    SDL_Log(buf);*/
}
template<typename... Args> void info(int category, const char *fmt, Args... args) {

}
template<typename... Args> void debug(int category, const char *fmt, Args... args) {

}
template<typename... Args> void warn(int category, const char *fmt, Args... args) {

}
template<typename... Args> void error(int category, const char *fmt, Args... args) {
   
}

template<typename... Args> void log(const std::string &fmt, Args... args) {
    
}
template<typename... Args> void info(int category, const std::string &fmt, Args... args) {

}
template<typename... Args> void debug(int category, const std::string &fmt, Args... args) {

}
template<typename... Args> void warn(int category, const std::string &fmt, Args... args) {

}
template<typename... Args> void error(int category, const std::string &fmt, Args... args) {

}