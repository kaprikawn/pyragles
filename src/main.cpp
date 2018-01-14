#include <SDL2/SDL.h>
#include <iostream>
#include "game.hpp"

int main( int argc, char* args[] ) {

  Uint32  currentTime_;
  Uint32  previousTime_;
  Uint32  msFrameDiff_ = 0;
  //float   dt_ = 0.0f;
  
  const int   WIDTH         = 1280;
  const int   HEIGHT        = 720;
  const int   FPS           = 60;
  const float DELAY_TIME    = 1000.0f / FPS;
  
  if( !TheGame::Instance() -> init( "GLES2 Test", 20, 20, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ) ) {
    std::cout << "Game initialisation failed" << std::endl;
  }
  
  while( TheGame::Instance() -> gameRunning() ) {
    previousTime_ = currentTime_;
    currentTime_ = SDL_GetTicks();
    msFrameDiff_ = currentTime_ - previousTime_;
    //dt_ = msFrameDiff_ / 1000.0f; 
    
    TheGame::Instance() -> handleInputs();
    TheGame::Instance() -> update();
    TheGame::Instance() -> render();
    
    if( msFrameDiff_ < DELAY_TIME ) {
      SDL_Delay( ( DELAY_TIME - msFrameDiff_ ) );
    }
  }

  return 0;
}
