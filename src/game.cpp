#include "game.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "gameStateMachine.hpp"

Game::Game( bool fullscreen ) {
  
  int windowWidth   = 1280;
  int windowHeight  = 720;
  
  Uint32 sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if( fullscreen ) {
    sdlFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode( 0, &dm );
    //windowWidth   = dm.w;
    //windowHeight  = dm.h;
    
  }
  
  bool init = Game::init( "GLES2 Test", 20, 20, windowWidth, windowHeight, sdlFlags );
  
  if( !init )
    std::cout << "Error - game failed to start" << std::endl;
  
}

bool Game::init( const char* title, int xpos, int ypos, int windowWidth, int windowHeight, int flags ) {
  
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ) {
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
  glViewport( 0, 0, windowWidth, windowHeight );
  
  gameStateMachine_ = std::make_unique<GameStateMachine>();
  inputHandler_     = std::make_shared<InputHandler>();
  camera_           = std::make_shared<Camera>( windowWidth, windowHeight );
  
  inputHandler_ -> initialiseGamepads();
  
  Game::changeGameState( PLAY, 0 );
  
  running_ = true;
  
  return true;
}

void Game::run() {
  
  Uint32  currentTime = SDL_GetTicks();
  Uint32  previousTime;
  GLfloat dt;
  
  if( !running_ )
    return;
  
  do {
    
    previousTime = currentTime;
    currentTime = SDL_GetTicks();
    dt = ( currentTime - previousTime ) / 1000.0f;
    if( dt > 0.15f )
      dt = 0.15f;
    
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
      inputHandler_ -> processEvent( event, currentTime );
    }
    
    if( inputHandler_ -> quit() )
      running_ = false;
      
    gameStateMachine_ -> update( dt );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    gameStateMachine_ -> render();
    
    Game::render();
  } while( running_ );
  
}

void Game::render() {
  SDL_GL_SwapWindow( window_ );
}

void Game::setNewState( int newState, int transitionType = 0 ) {
  newState_ = newState;
  transitionType_ = transitionType;
}

void Game::changeGameState( int newState, int transitionType ) {
  if( newState == PLAY ) {
    std::unique_ptr<GameState> playState ( std::make_unique<PlayState>() );
    gameStateMachine_ -> changeState( std::move( playState ), inputHandler_, camera_ );
  }
  newState_ = -1;
}