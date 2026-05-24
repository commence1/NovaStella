#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <atomic>
#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

extern void play_mp3(const std::string &filePath);
extern void stop_playback();
extern float get_playback_progress();
extern std::atomic<bool> is_playing;
extern std::atomic<int> audio_duration_ms;
extern std::atomic<int> audio_playback_ms;
