#define SDL_MAIN_USE_CALLBACKS

#include <vulkan/vulkan.hpp>

#include<glm/glm.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window *window;
SDL_Renderer *render;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **args) {
    if (!SDL_CreateWindowAndRenderer("Nova Stella", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_EXTERNAL | SDL_WINDOW_RESIZABLE, &window, &render)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!window || !render) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window or renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize audio: %s", SDL_GetError());
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
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
    SDL_Delay(16);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (render)
        SDL_DestroyRenderer(render);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}