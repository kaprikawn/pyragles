#include <iostream>
#include <GLES2/gl2.h>
#include "game.hpp"
#include "inputHandler.hpp"
#include "structs.hpp"
#include "playState.hpp"

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
  
  TheInputHandler::Instance() -> initialiseGamepads();
  
  std::unique_ptr<GameStateMachine> gameStateMachine ( new GameStateMachine );
  gameStateMachine_ = std::move( gameStateMachine );
  
  Game::changeGameState( PLAY, 0 );
  
  running_ = true;
  
  return true;
}

void Game::handleInputs() {
  TheInputHandler::Instance() -> update();
}

void Game::update( float dt ) {
  if( dt > 0.15f ) {
    dt = 0.15f;
  }

  gameStateMachine_ -> update( dt );
}

void Game::render() {
  gameStateMachine_ -> render();
}

void Game::setNewState( int newState, int transitionType = 0 ) {
  printf( "setting new state to %d\n", newState );
  newState_ = newState;
  transitionType_ = transitionType;
}

void Game::changeGameState( int newState, int transitionType ) {
  if( newState == PLAY ) {
    std::unique_ptr<GameState> playState ( new PlayState );
    gameStateMachine_ -> changeState( std::move( playState ) );
  }
  /*} else if( newState == TRANSITION ) {
    
    std::unique_ptr<GameState> transitionState ( new TransitionState( transitionType ) );
    gameStateMachine_ -> changeState( std::move( transitionState ) );
  } else if( newState == MENU ) {
    std::unique_ptr<GameState> menuState ( new MenuState );
    gameStateMachine_ -> changeState( std::move( menuState ) );
  }*/
  newState_ = -1;
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

