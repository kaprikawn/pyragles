#include <windows.h>
#include "game.hpp"
#include "game.cpp"

int CALLBACK WinMain( HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode ) {
  
  
#if DEBUG
  LPVOID baseAddress = ( LPVOID )Terabytes( 2 );
#else
  LPVOID baseAddress = 0;
#endif
  
  game_memory memory  = {};
  memory.permanentStorageSize = Megabytes( 64 );
  memory.tempStorageSize      = Gigabytes( 1 );
  
  uint64 totalMemorySize = memory.permanentStorageSize + memory.tempStorageSize;
  
  memory.permanentStorage     = VirtualAlloc( baseAddress, totalMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE );
  memory.tempStorage          = ( ( uint8 * )memory.permanentStorage + memory.permanentStorageSize );
  
  Assert( memory.permanentStorage );
  Assert( memory.tempStorage );
  
  init_game( &memory );
  
  return 0;
}