#include <init.hpp>

#define MINIMP3_IMPLEMENTATION

#include <minimp3/minimp3.h>

std::vector<unsigned char> buffer;

void play_mp3(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file.");
        return;
    }
    const auto size = file.tellg();

    buffer = std::vector<unsigned char>(size);

    file.seekg(0, std::ios::beg);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error: Failed to read file");
        return;
    }

    static mp3dec_t mp3d;
    mp3dec_init(&mp3d);

    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    int mp3len = 0;

    mp3dec_frame_info_t info;
    int samples = mp3dec_decode_frame(&mp3d, buffer.data(), size, pcm, &info);

    while (samples) {
        
        mp3len += info.frame_bytes;
        samples = mp3dec_decode_frame(&mp3d, buffer.data() + mp3len, static_cast<int>(size) - mp3len, pcm, &info);
    }
}

void play() {
    
}

void audio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
    (void)userdata;
    size_t remaining = (buffer.size());
}