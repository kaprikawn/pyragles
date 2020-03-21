#include "game.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "gameStateMachine.hpp"
#include "camera.hpp"
#include "inputHandler.hpp"

Game::Game( bool fullscreen, bool invertY ) {
  
  int windowWidth   = 1280;
  int windowHeight  = 720;
  
  int windowX       = 10;
  int windowY       = 10;
  
  Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if( fullscreen ) {
    sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
    
    // create window to native desktop size to query screen dimensions
    SDL_Init( SDL_INIT_VIDEO );
    SDL_Window* nullWindow = SDL_CreateWindow( "undef", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN );
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode( 0, &dm );
    windowWidth   = dm.w;
    windowHeight  = dm.h;
    SDL_DestroyWindow( nullWindow );
    
    windowX       = SDL_WINDOWPOS_UNDEFINED;
    windowY       = SDL_WINDOWPOS_UNDEFINED;
    
    SDL_ShowCursor( SDL_DISABLE );
  }
  
  bool init = Game::init( "GLES2 Test", windowX, windowY, windowWidth, windowHeight, sdlFlags, invertY );
  
  if( !init )
    std::cout << "Error - game failed to start" << std::endl;
  
}

bool Game::init( const char* title, int xpos, int ypos, int windowWidth, int windowHeight, int flags, bool invertY ) {
  
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ) != 0 ) {
    std::cout << "Failed to load SDL : " << SDL_GetError() << std::endl;
    return false;
  }
  
  window_ = SDL_CreateWindow( title, xpos, ypos, windowWidth, windowHeight, flags );
  if( !window_ ) {
    std::cout << "Failed to create window : " << SDL_GetError() << std::endl;
    return false;
  }
  
  glContext_ = SDL_GL_CreateContext( window_ );
  if( !glContext_ ) {
    std::cout << "Failed to create GL context : " << SDL_GetError() << std::endl;
    return false;
  }
  SDL_GL_SetSwapInterval( 1 );
  
  glClearColor( 0.0f, 0.65f, 1.0f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  
  gameStateMachine_ = std::make_unique<GameStateMachine>();
  InputHandler::Instance() -> init( invertY );
  
  Camera::Instance() -> init( windowWidth, windowHeight );
  
  InputHandler::Instance() -> initialiseGamepads();
  
  bool loadSuccessful = Game::changeGameState( PLAY, 0 );
  if( !loadSuccessful )
    return false;
  
  running_ = true;
  //running_ = false;
  
  return true;
}

void Game::run() {
  
  Uint32  currentTime = SDL_GetTicks();
  Uint32  previousTime;
  GLfloat dt;
  
  if( !running_ )
    return;
  
  do {
    
    previousTime  = currentTime;
    currentTime   = SDL_GetTicks();
    dt = ( currentTime - previousTime ) / 1000.0f;
    if( dt > 0.15f )
      dt = 0.15f;
    
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      InputHandler::Instance() -> processEvent( event, currentTime );
    }
    
    if( InputHandler::Instance() -> quit() )
      running_ = false;
      
    gameStateMachine_ -> update( dt );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    gameStateMachine_ -> render();
    
    Game::render();
    
    InputHandler::Instance() -> reset();
  } while( running_ );
  
}

void Game::render() {
  SDL_GL_SwapWindow( window_ );
}

void Game::setNewState( int newState, int transitionType = 0 ) {
  newState_ = newState;
  transitionType_ = transitionType;
}

bool Game::changeGameState( int newState, int transitionType ) {
  if( newState == PLAY ) {
    std::unique_ptr<GameState> playState ( std::make_unique<PlayState>() );
    bool changeSuccessful = gameStateMachine_ -> changeState( std::move( playState ) );
    if( !changeSuccessful )
      return false;
  }
  newState_ = -1;
  return true;
}