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
  
  game_memory memory  = {};
  memory.permanentStorageSize = Megabytes( 64 );
  memory.tempStorageSize      = Gigabytes( 1 );
  
  uint64 totalMemorySize = memory.permanentStorageSize + memory.tempStorageSize;
  
  memory.permanentStorage     = mmap( 0, memory.permanentStorageSize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0 );
  memory.tempStorage          = ( ( uint8 * )memory.permanentStorage + memory.permanentStorageSize );
  
  Assert( memory.permanentStorage );
  Assert( memory.tempStorage );
  
  init_game( &memory );
  
  return 0;
}
