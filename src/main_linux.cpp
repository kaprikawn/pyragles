#include <sys/mman.h>
#include "game.hpp"
#include "game.cpp"

int main( int argc, char* argv[] ) {
  
  for ( int i = 1; i < argc; i++ ) {
    if( strcmp( argv[i], "--fullscreen" ) == 0 )
      launch_fullscreen = true;
      
    // if( strcmp( argv[i], "--invertY" ) == 0 )
    //   invertY = true;
  }
  
  int32 game_ret_val = run_game();
  
  return 0;
}
