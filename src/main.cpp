#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <logger.hpp>
#include <init.hpp>

SDL_Window *window;
SDL_Renderer *render;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* iconTexture = NULL;
SDL_Texture* leftButtonTexture = NULL;
SDL_Texture* centerButtonTexture = NULL;
SDL_Texture* centerButtonTexture2 = NULL;
SDL_Texture* rightButtonTexture = NULL;
int centerButtonState = 0;
static const SDL_FRect rect_left_button = { 80, 250, 50, 50 };
static const SDL_FRect rect_center_button = { 250, 250, 70, 70 };
static const SDL_FRect rect_right_button = { 440, 250, 50, 50 };
static void render_button(const SDL_FRect *rect, const char *str, int button_value);
static void SDL_RenderCircle(SDL_Renderer *render, float x, float y, float radius);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *args[]) {
    if (!SDL_CreateWindowAndRenderer("Nova Stella", 570, 335, SDL_WINDOW_EXTERNAL, &window, &render)) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    backgroundTexture = IMG_LoadTexture(render, "image/context.jpg");
    if (!backgroundTexture) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载背景图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_Surface* iconSurface = IMG_Load("image/icon.jpg");
    if (!iconSurface) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载图标失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    leftButtonTexture = IMG_LoadTexture(render, "image/left_button.png");
    if (!leftButtonTexture) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载左按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    centerButtonTexture = IMG_LoadTexture(render, "image/center_button.png");
    if (!centerButtonTexture) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载中按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    centerButtonTexture2 = IMG_LoadTexture(render, "image/center_button2.png");
    if (!centerButtonTexture2) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载第二张中按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    rightButtonTexture = IMG_LoadTexture(render, "image/right_button.png");
    if (!rightButtonTexture) {
        logger::error(SDL_LOG_CATEGORY_VIDEO, "加载右按钮图片失败: %s", SDL_GetError());
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
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float left_distance = SDL_powf(event->button.x - (rect_left_button.x + rect_left_button.w / 2), 2) +
                                 SDL_powf(event->button.y - (rect_left_button.y + rect_left_button.h / 2), 2);
            float center_distance = SDL_powf(event->button.x - (rect_center_button.x + rect_center_button.w / 2), 2) +
                                   SDL_powf(event->button.y - (rect_center_button.y + rect_center_button.h / 2), 2);
            float right_distance = SDL_powf(event->button.x - (rect_right_button.x + rect_right_button.w / 2), 2) +
                                  SDL_powf(event->button.y - (rect_right_button.y + rect_right_button.h / 2), 2);
#ifdef DEBUG
            if (left_distance <= SDL_powf(rect_left_button.w / 2, 2)) {
                logger::log("点击了左按钮");
            } else if (center_distance <= SDL_powf(rect_center_button.w / 2, 2)) {
                logger::log("点击了中按钮");
                centerButtonState = !centerButtonState;
            } else if (right_distance <= SDL_powf(rect_right_button.w / 2, 2)) {
                logger::log("点击了右按钮");
            }
#endif
            break;
        }
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
    if (leftButtonTexture) SDL_DestroyTexture(leftButtonTexture);
    if (centerButtonTexture) SDL_DestroyTexture(centerButtonTexture);
    if (centerButtonTexture2) SDL_DestroyTexture(centerButtonTexture2);
    if (rightButtonTexture) SDL_DestroyTexture(rightButtonTexture);
    SDL_Quit();
}

static void render_button(const SDL_FRect *rect, const char *str, int button_value)
{
    SDL_Texture* buttonTexture = NULL;
    switch(button_value) {
        case -1: buttonTexture = leftButtonTexture; break;
        case 0: 
            if (centerButtonState == 0) {
                buttonTexture = centerButtonTexture;
            } else {
                buttonTexture = centerButtonTexture2;
            }
            break;
        case 1: buttonTexture = rightButtonTexture; break;
    }
    
    if (buttonTexture) {
        SDL_RenderTexture(render, buttonTexture, NULL, rect);
    }
}



static void SDL_RenderCircle(SDL_Renderer *render, float x, float y, float radius)
{
    int32_t cx = 0;
    int32_t cy = (int32_t)radius;
    int32_t error = -cy;

    while (cx <= cy) {
        SDL_RenderLine(render, x - cy, y + cx, x + cy, y + cx);
        SDL_RenderLine(render, x - cy, y - cx, x + cy, y - cx);
        SDL_RenderLine(render, x - cx, y + cy, x + cx, y + cy);
        SDL_RenderLine(render, x - cx, y - cy, x + cx, y - cy);

        cx++;
        error += cx + cx + 1;
        if (error >= 0) {
            cy--;
            error -= cy + cy + 1;
        }
    }
}

