#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include <init.hpp>
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>
#include <SDL3/SDL.h>

SDL_AudioDeviceID audio_device;
SDL_AudioStream* audio_stream;
int audio_duration_ms = 0;
std::atomic<int> audio_playback_ms{0};

static bool init_sdl_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_Log("Failed to initialize SDL audio subsystem: %s", SDL_GetError());
        return false;
    }
    return true;
}

void play_mp3(const std::string &filePath) {
    static mp3dec_t mp3d;
    static bool sdl_initialized = init_sdl_audio();
    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_Log("音频子系统未初始化！");
        return;
    }
    mp3dec_init(&mp3d);
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Failed to open file.");
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    mp3dec_frame_info_t info;
    int mp3len = 0;

    std::vector<int16_t> full_pcm_data;
    int total_samples = 0;
    int sample_rate = 0;
    int channels = 0;

    while (true) {
        int samples = mp3dec_decode_frame(&mp3d, buffer.data() + mp3len, size - mp3len, pcm, &info);
        if (samples <= 0) break;
        full_pcm_data.insert(full_pcm_data.end(), pcm, pcm + samples);
        mp3len += info.frame_bytes;
        total_samples += samples;
        sample_rate = info.hz;
        channels = info.channels;
    }

    audio_duration_ms = total_samples * 1000 / sample_rate;

    SDL_AudioSpec spec{};
    spec.freq = sample_rate;
    spec.format = SDL_AUDIO_S16;
    spec.channels = static_cast<Uint8>(channels);

    if (!audio_device) {
        audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!audio_device) {
            SDL_Log("Failed to open audio device: %s", SDL_GetError());
            return;
        }
    }

    if (audio_stream) SDL_DestroyAudioStream(audio_stream);
    audio_stream = SDL_CreateAudioStream(&spec, &spec);
    if (!audio_stream) {
        SDL_Log("Failed to create audio stream: %s", SDL_GetError());
        return;
    }
    
    SDL_ResumeAudioDevice(audio_device);
    SDL_PutAudioStreamData(audio_stream, full_pcm_data.data(), full_pcm_data.size() * sizeof(int16_t));
    SDL_FlushAudioStream(audio_stream);
    
    std::thread([&](){
        audio_playback_ms = 0;
        while (SDL_GetAudioStreamAvailable(audio_stream) > 0) {
            SDL_Delay(50);
            audio_playback_ms += 50;
        }
        audio_playback_ms = audio_duration_ms;
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = nullptr;
    }).detach();
}
