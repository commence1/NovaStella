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
extern void play_next();
extern void play_previous();
extern void play_track_at(int index);
extern void set_volume(int volume);
extern int get_current_track_index();
extern std::string get_current_track_name();
extern int get_track_count();
