#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>
#include <SDL3/SDL.h>
#include <atomic>
#include <vector>
#include <fstream>
#include <thread>
#include <stdexcept>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <filesystem>

SDL_AudioDeviceID audio_device = 0;     
SDL_AudioStream* audio_stream = nullptr;  
std::atomic<int> audio_duration_ms{0};  
std::atomic<int> audio_playback_ms{0};    
std::atomic<bool> is_playing{false};   
static std::thread playback_thread;
static std::mutex audio_mutex;
static std::atomic<bool> should_stop{false};
static std::atomic<int> current_volume{100};
static std::atomic<int> current_track_index{-1};
static std::vector<std::string> playlist;
static std::vector<int> track_durations;

void cleanup_audio() {
    should_stop = true;
    if (playback_thread.joinable()) {
        playback_thread.join();
    }
    std::lock_guard<std::mutex> lock(audio_mutex);
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = nullptr;
    }
    if (audio_device) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
    is_playing = false;
    should_stop = false;
}

bool load_mp3_file(const std::string& filePath, std::vector<int16_t>& pcm_data, int& sample_rate, int& channels, int& total_samples) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "无法打开文件: %s", filePath.c_str());
        return false;
    }

    const size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "无法读取文件: %s", filePath.c_str());
        return false;
    }

    mp3dec_t mp3d;
    mp3dec_init(&mp3d);
    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    mp3dec_frame_info_t info;
    int mp3len = 0;
    total_samples = 0;
    sample_rate = 0;
    channels = 0;

    while (true) {
        int samples = mp3dec_decode_frame(&mp3d, buffer.data() + mp3len, 
                                        static_cast<int>(size) - mp3len, pcm, &info);
        if (samples <= 0) break;

        pcm_data.insert(pcm_data.end(), pcm, pcm + samples * info.channels);
        mp3len += info.frame_bytes;
        total_samples += samples;
        sample_rate = info.hz;
        channels = info.channels;
    }

    return total_samples > 0;
}

void playback_loop(int total_bytes, int bytes_per_ms, int duration) {
    while (!should_stop && is_playing) {
        int available = SDL_GetAudioStreamAvailable(audio_stream);
        int played_bytes = total_bytes - available;
        
        if (played_bytes >= total_bytes || (available <= 0 && played_bytes > 0)) {
            audio_playback_ms.store(duration);
            is_playing.store(false);
            
            int next_index = current_track_index.load() + 1;
            if (next_index < static_cast<int>(playlist.size())) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                play_track(next_index);
            }
            break;
        }
        
        audio_playback_ms = played_bytes / bytes_per_ms;
        SDL_Delay(50);
    }
}

void play_track(int index) {
    if (index < 0 || index >= static_cast<int>(playlist.size())) {
        return;
    }

    cleanup_audio();

    std::vector<int16_t> pcm_data;
    int sample_rate = 0, channels = 0, total_samples = 0;

    if (!load_mp3_file(playlist[index], pcm_data, sample_rate, channels, total_samples)) {
        return;
    }

    int duration = total_samples * 1000 / sample_rate;
    audio_duration_ms.store(duration);
    audio_playback_ms = 0;
    is_playing = true;
    current_track_index = index;

    SDL_AudioSpec spec{};
    spec.freq = sample_rate;
    spec.format = SDL_AUDIO_S16;
    spec.channels = static_cast<Uint8>(channels);

    {
        std::lock_guard<std::mutex> lock(audio_mutex);
        audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
        if (!audio_device) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "无法打开音频设备: %s", SDL_GetError());
            return;
        }

        audio_stream = SDL_CreateAudioStream(&spec, &spec);
        if (!audio_stream) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "无法创建音频流: %s", SDL_GetError());
            return;
        }

        SDL_BindAudioStream(audio_device, audio_stream);
        SDL_SetAudioStreamVolume(audio_stream, current_volume.load() / 100.0f);
        SDL_ResumeAudioDevice(audio_device);
        SDL_PutAudioStreamData(audio_stream, pcm_data.data(), 
                              static_cast<int>(pcm_data.size() * sizeof(int16_t)));
        SDL_FlushAudioStream(audio_stream);
    }

    const int bytes_per_ms = (sample_rate * channels * sizeof(int16_t)) / 1000;
    const int total_bytes = static_cast<int>(pcm_data.size() * sizeof(int16_t));

    playback_thread = std::thread([total_bytes, bytes_per_ms, duration]() {
        playback_loop(total_bytes, bytes_per_ms, duration);
    });
}

void play_mp3(const std::string &filePath) {
    load_playlist("audio");
    if (!playlist.empty()) {
        int index = find_track_index(filePath);
        if (index >= 0) {
            play_track(index);
        } else {
            play_track(0);
        }
    }
}

void play_track_at(int index) {
    if (index >= 0 && index < static_cast<int>(playlist.size())) {
        play_track(index);
    }
}

void play_next() {
    int next = current_track_index.load() + 1;
    if (next >= static_cast<int>(playlist.size())) {
        next = 0;
    }
    play_track(next);
}

void play_previous() {
    int prev = current_track_index.load() - 1;
    if (prev < 0) {
        prev = static_cast<int>(playlist.size()) - 1;
    }
    if (prev >= 0) {
        play_track(prev);
    }
}

int find_track_index(const std::string& path) {
    for (int i = 0; i < static_cast<int>(playlist.size()); i++) {
        if (playlist[i] == path) return i;
    }
    return -1;
}

void load_playlist(const std::string& directory) {
    playlist.clear();
    std::filesystem::path dir(directory);
    if (!std::filesystem::exists(dir)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "音频目录不存在: %s", directory.c_str());
        return;
    }

    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".mp3" || ext == ".wav" || ext == ".ogg") {
                files.push_back(entry.path().string());
            }
        }
    }
    
    std::sort(files.begin(), files.end());
    playlist = std::move(files);
    
    SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "加载了 %d 首曲目", playlist.size());
}

void set_volume(int volume) {
    volume = std::max(0, std::min(100, volume));
    current_volume.store(volume);
    
    if (audio_stream) {
        SDL_SetAudioStreamVolume(audio_stream, volume / 100.0f);
    }
}

void stop_playback() {
    is_playing = false;
    cleanup_audio();
    audio_playback_ms = 0;
}

float get_playback_progress() {
    int duration = audio_duration_ms.load();
    if (duration == 0) return 0.0f;
    return static_cast<float>(audio_playback_ms.load()) / duration;
}

int get_current_track_index() {
    return current_track_index.load();
}

std::string get_current_track_name() {
    int index = current_track_index.load();
    if (index >= 0 && index < static_cast<int>(playlist.size())) {
        return std::filesystem::path(playlist[index]).filename().string();
    }
    return "";
}

int get_track_count() {
    return static_cast<int>(playlist.size());
}
