#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <logger.hpp>

SDL_Window *window;
SDL_Renderer *render;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* iconTexture = NULL;
SDL_Texture* leftButtonTexture = NULL;
SDL_Texture* centerButtonTexture = NULL;
SDL_Texture* centerButtonTexture2 = NULL;
SDL_Texture* rightButtonTexture = NULL;
SDL_Texture* blurTexture = NULL;
SDL_Texture* barTexture = NULL;
SDL_Surface* iconSurface = NULL;
static int centerButtonState = 0;
bool leftButtonHover = false;
bool centerButtonHover = false;
bool rightButtonHover = false;
static bool isPlaying = false;
static const SDL_FRect rect_left_button = { 50, 280, 25, 25 };
static const SDL_FRect rect_center_button = { 120, 280, 25, 25 };
static const SDL_FRect rect_right_button = { 190, 280, 25, 25 };
static float audio_duration_ms = 0.0f;
static float audio_playback_ms = 0.0f;
static void render_button(const SDL_FRect *rect, const char *str, int button_value, bool isHover);
static void SDL_RenderCircle(SDL_Renderer *render, float x, float y, float radius);
void audio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount);
void play_mp3(const std::string &filePath);


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "视频初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Nova Stella", 570, 335, SDL_WINDOW_EXTERNAL, &window, &render)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "音频初始化失败: %s", SDL_GetError());
    }

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "事件系统初始化失败: %s", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        return SDL_APP_FAILURE;
    }

    backgroundTexture = IMG_LoadTexture(render, "image/context.jpg");
    if (!backgroundTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载背景图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    blurTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 570 / 4, 335 / 4);
    if (!blurTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "创建模糊纹理失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetTextureBlendMode(blurTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(blurTexture, 128);
    
    SDL_Surface* iconSurface = IMG_Load("image/icon.jpg");
    if (!iconSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载图标失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    barTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 570, 80);
    if (!barTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "创建长条纹理失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetTextureBlendMode(barTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(barTexture, 180);

    leftButtonTexture = IMG_LoadTexture(render, "image/left_button.png");
    if (!leftButtonTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载左按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    centerButtonTexture = IMG_LoadTexture(render, "image/center_button.png");
    if (!centerButtonTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载中按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    centerButtonTexture2 = IMG_LoadTexture(render, "image/center_button2.png");
    if (!centerButtonTexture2) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载第二张中按钮图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    rightButtonTexture = IMG_LoadTexture(render, "image/right_button.png");
    if (!rightButtonTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载右按钮图片失败: %s", SDL_GetError());
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
            if (center_distance <= SDL_powf(rect_center_button.w / 2, 2)) {
                centerButtonState = (centerButtonState + 1) % 2;
                if (!isPlaying) {
                    play_mp3("audio/music.mp3");
                    isPlaying = true;
                }
            }   
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            float left_distance = SDL_powf(event->motion.x - (rect_left_button.x + rect_left_button.w / 2), 2) +
                                SDL_powf(event->motion.y - (rect_left_button.y + rect_left_button.h / 2), 2);
            leftButtonHover = (left_distance <= SDL_powf(rect_left_button.w / 2, 2));        
            float center_distance = SDL_powf(event->motion.x - (rect_center_button.x + rect_center_button.w / 2), 2) +
                                SDL_powf(event->motion.y - (rect_center_button.y + rect_center_button.h / 2), 2);
            centerButtonHover = (center_distance <= SDL_powf(rect_center_button.w / 2, 2));
            float right_distance = SDL_powf(event->motion.x - (rect_right_button.x + rect_right_button.w / 2), 2) +
                                SDL_powf(event->motion.y - (rect_right_button.y + rect_right_button.h / 2), 2);
            rightButtonHover = (right_distance <= SDL_powf(rect_right_button.w / 2, 2));
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_RenderClear(render);
    
    SDL_RenderTexture(render, backgroundTexture, NULL, NULL);
    
    SDL_Texture* target = SDL_GetRenderTarget(render);
    SDL_SetRenderTarget(render, blurTexture);
    
    SDL_FRect smallRect = { 0, 0, 570 / 4, 335 / 4 };
    SDL_RenderTexture(render, backgroundTexture, NULL, &smallRect);
    
    SDL_SetRenderTarget(render, target);
    
    SDL_SetTextureBlendMode(blurTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(blurTexture, 180); 
    
    SDL_FRect blurRect = { 0, 0, 570, 335 };
    SDL_RenderTexture(render, blurTexture, NULL, &blurRect);
    
    SDL_SetRenderTarget(render, barTexture);
    SDL_SetRenderDrawColor(render, 135, 206, 250, 255);
    SDL_RenderClear(render);
    SDL_SetRenderTarget(render, target);
    
    SDL_FRect barRect = { 0, 273, 570, 40 };
    SDL_RenderTexture(render, barTexture, NULL, &barRect);

    render_button(&rect_left_button, "LEFT", -1, leftButtonHover);
    render_button(&rect_center_button, "CENTER", 0, centerButtonHover);
    render_button(&rect_right_button, "RIGHT", 1, rightButtonHover);

    if (isPlaying && audio_duration_ms > 0) {
        float progress = audio_playback_ms * 1.0f / audio_duration_ms;
        SDL_FRect progressBgRect = { 50, 320, 470, 5 };
        SDL_SetRenderDrawColor(render, 100, 100, 100, 255);
        SDL_RenderFillRect(render, &progressBgRect);
        SDL_FRect progressRect = { 50, 320, 470 * progress, 5};
        SDL_SetRenderDrawColor(render, 255, 182, 193, 255);
        SDL_RenderFillRect(render, &progressRect);
        if (isPlaying && audio_playback_ms >= audio_duration_ms) {
            isPlaying = false;
            centerButtonState = 0;
            progressRect.w = 470;
            SDL_RenderFillRect(render, &progressRect);
        }
    }

    SDL_RenderPresent(render);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (backgroundTexture)
        SDL_DestroyTexture(backgroundTexture);
    if (render)
        SDL_DestroyRenderer(render);
    if (window)
        SDL_DestroyWindow(window);
    if (iconTexture)
        SDL_DestroyTexture(iconTexture);
    if (leftButtonTexture)
        SDL_DestroyTexture(leftButtonTexture);
    if (centerButtonTexture)
        SDL_DestroyTexture(centerButtonTexture);
    if (centerButtonTexture2)
        SDL_DestroyTexture(centerButtonTexture2);
    if (rightButtonTexture)
        SDL_DestroyTexture(rightButtonTexture);
    if (iconSurface)
        SDL_DestroySurface(iconSurface);
    if (blurTexture)
        SDL_DestroyTexture(blurTexture);
    if (barTexture) 
        SDL_DestroyTexture(barTexture);
    SDL_Quit();
}


static void render_button(const SDL_FRect *rect, const char *str, int button_value, bool isHover) {
    SDL_Texture* buttonTexture = NULL;
    switch(button_value) {
        case -1: buttonTexture = leftButtonTexture; break;
        case 0: 
            if (centerButtonState == 0) {
                buttonTexture = centerButtonTexture;
            }else {
                buttonTexture = centerButtonTexture2;
            }
            break;
        case 1: buttonTexture = rightButtonTexture; break;
    }
    
    if (buttonTexture) {
        if (isHover) {
            SDL_FRect hoverRect = {
                rect->x - 5, 
                rect->y - 5, 
                rect->w + 10,
                rect->h + 10 
            };
            SDL_RenderTexture(render, buttonTexture, NULL, &hoverRect);
        } else {
            SDL_RenderTexture(render, buttonTexture, NULL, rect);
        }
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
