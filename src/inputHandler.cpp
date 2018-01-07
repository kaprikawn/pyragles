#include <iostream>
#include <vector>
#include "inputHandler.hpp"
#include "game.hpp"

InputHandler* InputHandler::instance_ = 0;

void InputHandler::onKeyDown() {
  keystates_ = SDL_GetKeyboardState( NULL );
  
  if( keystates_[ SDL_SCANCODE_ESCAPE ] == 1 ) {
    TheGame::Instance() -> quit();
  }
}

void InputHandler::update() {
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
      case SDL_QUIT:
        TheGame::Instance() -> quit(); break;
      case SDL_KEYDOWN:
        onKeyDown(); break;
      default:
        break;
    }
  }
}
