#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>
#include <SDL3/SDL.h>
#include <atomic>
#include <vector>
#include <fstream>
#include <thread>
#include <stdexcept>

SDL_AudioDeviceID audio_device = 0;     
SDL_AudioStream* audio_stream = nullptr;  
std::atomic<int> audio_duration_ms{0};  
std::atomic<int> audio_playback_ms{0};    
std::atomic<bool> is_playing{false};   

bool init_sdl_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_Log("初始化SDL音频子系统失败: %s", SDL_GetError());
        return false;
    }
    return true;
}

void cleanup_audio() {
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = nullptr;
    }
    if (audio_device) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
    is_playing = false;
}

void play_mp3(const std::string &filePath) {
    static bool sdl_initialized = init_sdl_audio();
    if (!sdl_initialized) {
        throw std::runtime_error("音频子系统初始化失败");
    }
    cleanup_audio();

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    const size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("读取文件失败: " + filePath);
    }

    mp3dec_t mp3d;
    mp3dec_init(&mp3d);
    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    mp3dec_frame_info_t info;
    int mp3len = 0;
    std::vector<int16_t> full_pcm_data;
    int total_samples = 0;
    int sample_rate = 0;
    int channels = 0;

    while (true) {
        int samples = mp3dec_decode_frame(&mp3d, buffer.data() + mp3len, 
                                        static_cast<int>(size) - mp3len, pcm, &info);
        if (samples <= 0) break;

        full_pcm_data.insert(full_pcm_data.end(), pcm, pcm + samples * info.channels);
        mp3len += info.frame_bytes;
        total_samples += samples;
        sample_rate = info.hz;
        channels = info.channels;
    }

    audio_duration_ms = total_samples * 1000 / sample_rate;
    audio_playback_ms = 0;
    is_playing = true;

    SDL_AudioSpec spec{};
    spec.freq = sample_rate;
    spec.format = SDL_AUDIO_S16;
    spec.channels = static_cast<Uint8>(channels);

    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!audio_device) {
        throw std::runtime_error("无法打开音频设备: " + std::string(SDL_GetError()));
    }

    audio_stream = SDL_CreateAudioStream(&spec, &spec);
    if (!audio_stream) {
        throw std::runtime_error("无法创建音频流: " + std::string(SDL_GetError()));
    }

    SDL_ResumeAudioDevice(audio_device);
    SDL_PutAudioStreamData(audio_stream, full_pcm_data.data(), 
                          full_pcm_data.size() * sizeof(int16_t));
    SDL_FlushAudioStream(audio_stream);

    std::thread([&]() {
        const int bytes_per_ms = (sample_rate * channels * sizeof(int16_t)) / 1000;
        const int total_bytes = full_pcm_data.size() * sizeof(int16_t);
        
        while (is_playing) {
            int available = SDL_GetAudioStreamAvailable(audio_stream);
            if (available <= 0) break;
            
            int played_bytes = total_bytes - available;
            audio_playback_ms = played_bytes / bytes_per_ms;
            
            SDL_Delay(50);
        }
        
        audio_playback_ms.store(audio_duration_ms.load());
        is_playing.store(false);
    }).detach();
}

void stop_playback() {
    is_playing = false;
    cleanup_audio();
}

float get_playback_progress() {
    if (audio_duration_ms == 0) return 0.0f;
    return static_cast<float>(audio_playback_ms) / audio_duration_ms;
}