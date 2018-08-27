#include "audio.hpp"
#include <iostream>

void Audio::load( const char* filename ) {
  SDL_LoadWAV( filename, &wavSpec_, &wavBuffer_, &wavLength_ );
  deviceID_ = SDL_OpenAudioDevice( NULL, 0, &wavSpec_, NULL, 0 );
}

void Audio::play() {
  SDL_QueueAudio( deviceID_, wavBuffer_, wavLength_ );
  SDL_PauseAudioDevice( deviceID_, 0 );
}