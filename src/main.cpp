#include <iostream>
#include <memory>
#include "game.hpp"

int main( int argc, char* argv[] ) {
  
  bool fullscreen = false;
  bool invertY    = false;
  
  for ( int i = 1; i < argc; i++ ) {
    if( strcmp( argv[i], "--fullscreen" ) == 0 )
      fullscreen = true;
      
    if( strcmp( argv[i], "--invertY" ) == 0 )
      invertY = true;
  }
  
  std::shared_ptr<Game> game = std::make_shared<Game>( fullscreen, invertY );
  
  game -> run();
  
  return 0;
}