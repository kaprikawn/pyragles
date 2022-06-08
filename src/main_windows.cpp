#include <windows.h>
#include "game.hpp"

int CALLBACK WinMain( HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode ) {
  
int32 game_ret_val = run_game();
  
  return 0;
}