#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_log.h>

SDL_Window *window;
SDL_Renderer *render;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* iconTexture = NULL;
static const SDL_FRect rect_left_button = { 80, 250, 50, 50 };
static const SDL_FRect rect_center_button = { 250, 250, 70, 70};
static const SDL_FRect rect_right_button = { 440, 250, 50, 50 };
static void render_button(const SDL_FRect *rect, const char *str, int button_value);


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *args[]) {
    if (!SDL_CreateWindowAndRenderer("Nova Stella", 570, 335, SDL_WINDOW_EXTERNAL, &window, &render)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    backgroundTexture = IMG_LoadTexture(render, "../image/context.jpg");
    if (!backgroundTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载背景图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Surface* iconSurface = IMG_Load("../image/icon.jpg");
    if (!iconSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载图标失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowIcon(window, iconSurface);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
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
    SDL_RenderClear(render);
    SDL_RenderTexture(render, backgroundTexture, NULL, NULL); 
    render_button(&rect_left_button, "LEFT", -1);
    render_button(&rect_center_button, "CENTER", 0);
    render_button(&rect_right_button, "RIGHT", 1);
    SDL_RenderPresent(render);
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (render) SDL_DestroyRenderer(render);
    if (window) SDL_DestroyWindow(window);
    if (iconTexture) SDL_DestroyTexture(iconTexture);
    SDL_Quit();
}
static void render_button(const SDL_FRect *rect, const char *str, int button_value)
{
    float x, y;
    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
    SDL_RenderFillRect(render, rect);
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    x = rect->x + ((rect->w - (SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(str))) / 2.0f);
    y = rect->y + ((rect->h - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2.0f);
    SDL_RenderDebugText(render, x, y, str);
}
