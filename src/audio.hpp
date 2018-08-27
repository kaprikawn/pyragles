#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Audio {
  private:
    SDL_AudioSpec       wavSpec_;
    Uint32              wavLength_;
    Uint8*              wavBuffer_;
    SDL_AudioDeviceID   deviceID_;
    
  public:
    Audio(){}
    ~Audio(){}
    
    void load( const char* filename );
    void play();
    
    
};

#endif //AUDIO_HPP