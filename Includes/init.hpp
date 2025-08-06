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