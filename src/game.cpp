#include <iostream>
#include <GLES2/gl2.h>
#include "game.hpp"
#include "triangle.hpp"

Game* Game::instance_ = 0;

bool Game::init( const char* title, int xpos, int ypos, int width, int height, int flags ) {

  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ) {
    std::cout << "Failed to load SDL : " << SDL_GetError() << std::endl;
    return false;
  }
  
  window_ = SDL_CreateWindow( title, xpos, ypos, width, height, flags );
  if( !window_ ) {
    std::cout << "Failed to create window : " << SDL_GetError() << std::endl;
    return false;
  }
  
  Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  renderer_ = SDL_CreateRenderer( window_, -1, renderFlags );
  if( !renderer_ ) {
    std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
    return false;
  }
  
  TheTriangle::Instance() -> init();
  
  running_ = true;
  
  return true;
}

void Game::handleInputs() {
  TheInputHandler::Instance() -> update();
}

void Game::update() {
  // placeholder
  return;
}

void Game::render() {
  TheTriangle::Instance() -> render();
}

void Game::clean() {
  SDL_DestroyWindow( window_ );
  SDL_DestroyRenderer( renderer_ );
  
  window_   = nullptr;
  renderer_ = nullptr;
}

void Game::quit() {
  TheGame::Instance() -> clean();
  std::cout << "Killing SDL" << std::endl;
  SDL_Quit();
  running_ = false;
}

