#define SDL_MAIN_USE_CALLBACKS

#include <vulkan-init.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include<glm/glm.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

VulkanInit vulkaninit;

SDL_Window *window{nullptr};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **args) {
#ifndef DEBUG
    boost::log::add_file_log(
        boost::log::keywords::file_name = "log_%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%]: %Message%"
    );
    boost::log::core::get()->set_filter(boost::log::trivial::severity>=boost::log::trivial::info);

    boost::log::add_common_attributes();

    boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
#endif
    //vulkaninit.vulkan_init();

    window = SDL_CreateWindow("Nova Stella", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    if (!window) {
#ifdef DEBUG
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window: %s", SDL_GetError());
#else
        BOOST_LOG_SEV(lg, boost::log::trivial::error) << "Failed to create window: " << SDL_GetError();
#endif
        return SDL_APP_FAILURE;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
#ifdef DEBUG
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize audio: %s", SDL_GetError());
#else
        BOOST_LOG_SEV(lg, boost::log::trivial::error) << "Failed to intialize audio: " << SDL_GetError();
#endif
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    //vulkaninit.interate();
    SDL_Delay(16);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

    //vulkaninit.clean();
    
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}