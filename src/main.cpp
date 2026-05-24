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
SDL_Texture* blurTexture = NULL;
SDL_Texture* barTexture = NULL;
SDL_Texture* volumeIconTexture = NULL;
SDL_Surface* iconSurface = NULL;
static int centerButtonState = 0;
bool leftButtonHover = false;
bool centerButtonHover = false;
bool rightButtonHover = false;
static int volume = 100;
static bool showVolumeBar = false;
static Uint64 lastVolumeShowTime = 0;
static const SDL_FRect rect_left_button = { 50, 280, 25, 25 };
static const SDL_FRect rect_center_button = { 120, 280, 25, 25 };
static const SDL_FRect rect_right_button = { 190, 280, 25, 25 };
static const SDL_FRect rect_volume_icon = { 520, 280, 20, 20 };
static const SDL_FRect rect_progress_bar = { 50, 315, 470, 8 };
static const SDL_FRect rect_volume_bar = { 470, 285, 80, 10 };
static void render_button(const SDL_FRect *rect, const char *str, int button_value, bool isHover);
static std::string format_time(int ms);
static void render_text(const char* text, float x, float y, Uint8 r, Uint8 g, Uint8 b);


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *args[]) {

    if (SDL_CreateWindowAndRenderer("Nova Stella", 570, 360, SDL_WINDOW_EXTERNAL, &window, &render)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    backgroundTexture = IMG_LoadTexture(render, "image/context.jpg");
    if (!backgroundTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "加载背景图片失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    blurTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 570 / 4, 360 / 4);
    if (!blurTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "创建模糊纹理失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetTextureBlendMode(blurTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(blurTexture, 128);
    
    iconSurface = IMG_Load("image/icon.jpg");
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

    return SDL_APP_SUCCESS;
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
            
            if (left_distance <= SDL_powf(rect_left_button.w / 2, 2)) {
                play_previous();
            } else if (center_distance <= SDL_powf(rect_center_button.w / 2, 2)) {
                if (!is_playing) {
                    play_mp3("audio/music.mp3");
                    centerButtonState = 1;
                } else {
                    stop_playback();
                    centerButtonState = 0;
                }
            } else if (right_distance <= SDL_powf(rect_right_button.w / 2, 2)) {
                play_next();
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
        case SDL_EVENT_MOUSE_WHEEL: {
            volume = SDL_clamp(volume + event->wheel.y * 5, 0, 100);
            set_volume(volume);
            showVolumeBar = true;
            lastVolumeShowTime = SDL_GetTicks64();
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
    
    SDL_FRect smallRect = { 0, 0, 570 / 4, 360 / 4 };
    SDL_RenderTexture(render, backgroundTexture, NULL, &smallRect);
    
    SDL_SetRenderTarget(render, target);
    
    SDL_SetTextureBlendMode(blurTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(blurTexture, 180); 
    
    SDL_FRect blurRect = { 0, 0, 570, 360 };
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

    if (audio_duration_ms.load() > 0) {
        float progress = get_playback_progress();
        
        SDL_FRect progressBgRect = rect_progress_bar;
        SDL_SetRenderDrawColor(render, 60, 60, 60, 255);
        SDL_RenderFillRect(render, &progressBgRect);
        
        SDL_FRect progressRect = { rect_progress_bar.x, rect_progress_bar.y, rect_progress_bar.w * progress, rect_progress_bar.h };
        SDL_SetRenderDrawColor(render, 255, 182, 193, 255);
        SDL_RenderFillRect(render, &progressRect);
        
        int currentTime = audio_playback_ms.load();
        int totalTime = audio_duration_ms.load();
        std::string currentTimeStr = format_time(currentTime);
        std::string totalTimeStr = format_time(totalTime);
        
        render_text(currentTimeStr.c_str(), rect_progress_bar.x, rect_progress_bar.y - 15, 255, 255, 255);
        render_text(totalTimeStr.c_str(), rect_progress_bar.x + rect_progress_bar.w - 35, rect_progress_bar.y - 15, 255, 255, 255);
        
        if (!is_playing.load() && currentTime >= totalTime) {
            centerButtonState = 0;
        }
    }
    
    std::string trackName = get_current_track_name();
    if (!trackName.empty()) {
        int trackIndex = get_current_track_index() + 1;
        int trackCount = get_track_count();
        std::string trackInfo = trackName;
        if (trackCount > 1) {
            trackInfo = fmt::format("{}/{} - {}", trackIndex, trackCount, trackName);
        }
        
        int textWidth = static_cast<int>(trackInfo.length() * 8);
        float textX = (570 - textWidth) / 2;
        render_text(trackInfo.c_str(), textX, 250, 255, 255, 255);
    }
    
    if (showVolumeBar || (SDL_GetTicks64() - lastVolumeShowTime < 2000)) {
        if (SDL_GetTicks64() - lastVolumeShowTime >= 2000) {
            showVolumeBar = false;
        }
        
        SDL_FRect volumeBarBg = rect_volume_bar;
        SDL_SetRenderDrawColor(render, 60, 60, 60, 255);
        SDL_RenderFillRect(render, &volumeBarBg);
        
        SDL_FRect volumeBar = { rect_volume_bar.x, rect_volume_bar.y, rect_volume_bar.w * (volume / 100.0f), rect_volume_bar.h };
        SDL_SetRenderDrawColor(render, 100, 200, 100, 255);
        SDL_RenderFillRect(render, &volumeBar);
        
        std::string volumeText = fmt::format("{}%", volume);
        render_text(volumeText.c_str(), rect_volume_bar.x + rect_volume_bar.w + 5, rect_volume_bar.y - 2, 255, 255, 255);
    }

    SDL_RenderPresent(render);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    stop_playback();
    if (backgroundTexture)
        SDL_DestroyTexture(backgroundTexture);
    if (blurTexture)
        SDL_DestroyTexture(blurTexture);
    if (barTexture) 
        SDL_DestroyTexture(barTexture);
    if (leftButtonTexture)
        SDL_DestroyTexture(leftButtonTexture);
    if (centerButtonTexture)
        SDL_DestroyTexture(centerButtonTexture);
    if (centerButtonTexture2)
        SDL_DestroyTexture(centerButtonTexture2);
    if (rightButtonTexture)
        SDL_DestroyTexture(rightButtonTexture);
    if (iconTexture)
        SDL_DestroyTexture(iconTexture);
    if (iconSurface)
        SDL_DestroySurface(iconSurface);
    if (render)
        SDL_DestroyRenderer(render);
    if (window)
        SDL_DestroyWindow(window);
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

static std::string format_time(int ms) {
    int seconds = (ms / 1000) % 60;
    int minutes = (ms / 1000) / 60;
    return fmt::format("{:02d}:{:02d}", minutes, seconds);
}

static void render_text(const char* text, float x, float y, Uint8 r, Uint8 g, Uint8 b) {
    if (!text) return;
    
    SDL_Color color = { r, g, b, 255 };
    SDL_Surface* surface = SDL_RenderGeometryRawUTF8(render, nullptr, text, -1, nullptr, 0, color);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
        if (texture) {
            int w, h;
            SDL_GetTextureSize(texture, &w, &h);
            SDL_FRect dst = { x, y, static_cast<float>(w), static_cast<float>(h) };
            SDL_RenderTexture(render, texture, NULL, &dst);
            SDL_DestroyTexture(texture);
        }
        SDL_DestroySurface(surface);
    }
}
