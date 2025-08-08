#define SDL_MAIN_USE_CALLBACKS

#include <boost/log/trivial.hpp>

#include <vulkan-init.hpp>

#include<glm/glm.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

VulkanInit vulkaninit;

SDL_Window *window{nullptr};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **args) {
    window = SDL_CreateWindow("Nova Stella", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    vulkaninit.vulkan_init();

    if (!window) {
#ifdef DEBUG
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window: %s", SDL_GetError());
#elif
        
#endif
        return SDL_APP_FAILURE;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
#ifdef DEBUG
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize audio: %s", SDL_GetError());
#elif
        
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
    vulkaninit.interate();
    SDL_Delay(16);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

    vulkaninit.clean();
    
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}