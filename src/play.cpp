#include <init.hpp>

void play_mp3(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file.");
        return;
    }
    const auto size = file.tellg();

    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error: Failed to read file");
        return;
    }

    static mp3dec_t mp3d;
    mp3dec_init(&mp3d);

    short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
    int mp3len = 0;

    mp3dec_frame_info_t info;
    int samples = mp3dec_decode_frame(&mp3d, buffer.data(), size, nullptr, &info);

    while (samples) {
        
        mp3len += info.frame_bytes;
        samples = mp3dec_decode_frame(&mp3d, buffer.data() + mp3len, size - mp3len, pcm, &info);
    }
}

void play() {
    
}