#ifndef INPUT_HPP
#define INPUT_HPP

#include "types.hpp"

#ifdef _WIN32

#include "SDL.h"

#elif __linux__

#include <SDL2/SDL.h>

#endif

struct Buttons_pressed {
  bool32 quit       = false;
  bool32 w          = false;
  bool32 a          = false;
  bool32 s          = false;
  bool32 d          = false;
  real32 joy_axis_x = 0.0f;
  real32 joy_axis_y = 0.0f;
};

struct Game_input {
  bool w_pressed    = false;
  bool w_held       = false;
  bool w_released   = false;
  bool a_pressed    = false;
  bool a_held       = false;
  bool a_released   = false;
  bool s_pressed    = false;
  bool s_held       = false;
  bool s_released   = false;
  bool d_pressed    = false;
  bool d_held       = false;
  bool d_released   = false;
  bool quit         = false;
  real32 joy_axis_x = 0.0f;
  real32 joy_axis_y = 0.0f;
};

void reset_game_inputs_pressed( Buttons_pressed* old_buttons, Buttons_pressed* new_buttons ) {
  old_buttons -> w = new_buttons -> w;
  old_buttons -> a = new_buttons -> a;
  old_buttons -> s = new_buttons -> s;
  old_buttons -> d = new_buttons -> d;
}

Game_input get_game_input_state( Buttons_pressed old_buttons, Buttons_pressed new_buttons ) {
  
  Game_input result;
  
  if( old_buttons.quit || new_buttons.quit )
    result.quit = true;
  
  if( !old_buttons.w && new_buttons.w ) {
    result.w_pressed  = true;
    result.w_held     = true;
  } else if( old_buttons.w && new_buttons.w ) {
    result.w_held     = true;
  } else if( old_buttons.w && !new_buttons.w ) {
    result.w_released = true;
  }
  
  if( !old_buttons.a && new_buttons.a ) {
    result.a_pressed  = true;
    result.a_held     = true;
  } else if( old_buttons.a && new_buttons.a ) {
    result.a_held     = true;
  } else if( old_buttons.a && !new_buttons.a ) {
    result.a_released = true;
  }
  
  if( !old_buttons.s && new_buttons.s ) {
    result.s_pressed  = true;
    result.s_held     = true;
  } else if( old_buttons.s && new_buttons.s ) {
    result.s_held     = true;
  } else if( old_buttons.s && !new_buttons.s ) {
    result.s_released = true;
  }
  
  if( !old_buttons.d && new_buttons.d ) {
    result.d_pressed  = true;
    result.d_held     = true;
  } else if( old_buttons.d && new_buttons.d ) {
    result.d_held     = true;
  } else if( old_buttons.d && !new_buttons.d ) {
    result.d_released = true;
  }
  
  result.joy_axis_x = new_buttons.joy_axis_x;
  result.joy_axis_y = new_buttons.joy_axis_y;
  
  return result;
}

void input_on_key_down( SDL_Event* event, Buttons_pressed* old_buttons, Buttons_pressed* new_buttons ) {
  
  if( event -> key.repeat > 0 )
    return;
  
  uint32 scancode = event -> key.keysym.scancode;
  
  switch( scancode ) {
    
    case SDL_SCANCODE_ESCAPE : {
      new_buttons -> quit = true;
      old_buttons -> quit = false;
    } break;
    
    case SDL_SCANCODE_W : {
      new_buttons -> w = true;
      old_buttons -> w = false;
    } break;
    
    case SDL_SCANCODE_A : {
      new_buttons -> a = true;
      old_buttons -> a = false;
    } break;
    
    case SDL_SCANCODE_S : {
      new_buttons -> s = true;
      old_buttons -> s = false;
    } break;
    
    case SDL_SCANCODE_D : {
      new_buttons -> d = true;
      old_buttons -> d = false;
    } break;
    
    default : break;
  }
}

void input_on_key_up( SDL_Event* event, Buttons_pressed* old_buttons, Buttons_pressed* new_buttons ) {
  
  uint32 scancode = event -> key.keysym.scancode;
  
  switch( scancode ) {
    
    case SDL_SCANCODE_ESCAPE : {
      new_buttons -> quit = false;
      old_buttons -> quit = true;
    } break;
    
    case SDL_SCANCODE_W : {
      new_buttons -> w = false;
      old_buttons -> w = true;
    } break;
    
    case SDL_SCANCODE_A : {
      new_buttons -> a = false;
      old_buttons -> a = true;
    } break;
    
    case SDL_SCANCODE_S : {
      new_buttons -> s = false;
      old_buttons -> s = true;
    } break;
    
    case SDL_SCANCODE_D : {
      new_buttons -> d = false;
      old_buttons -> d = true;
    } break;
    
    default : break;
  }
}

uint32 gamepad_count          = 0;
const uint32 MAX_CONTROLLERS  = 4;
SDL_GameController* gamepads[ MAX_CONTROLLERS ];

void initialise_gamepads() {
  
  // SDL_Joystick;
  
  if( SDL_WasInit( SDL_INIT_GAMECONTROLLER ) == 0 ) {
    SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER );
  }
  
  gamepad_count = SDL_NumJoysticks();
  
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Detected %d attached gamepads\n", gamepad_count );
  
  if( gamepad_count == 0 ) return;
  
  SDL_GameController* gamepad = NULL;
  
  for( uint32 i = 0; i < gamepad_count && i < MAX_CONTROLLERS; i++ ) {
    if( SDL_IsGameController( i ) ) {
      gamepad = SDL_GameControllerOpen( i );
      if( gamepad ) {
        gamepads[ i ] = gamepad;
        SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Added controller %s\n", SDL_GameControllerName( gamepad ) );
      } else {
        SDL_LogInfo( SDL_LOG_CATEGORY_ERROR, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
      }
    }
  }
}

int16 deadzone = 100;

void on_joy_axis_move( SDL_Event* event, Buttons_pressed* new_buttons ) {
  
  int16   axis_value  = event -> jaxis.value;
  real32  normalised_value;
  
  if( axis_value < deadzone && axis_value > -deadzone ) {
    normalised_value = 0.0f;
  } else if( axis_value > 0 ) {
    normalised_value = ( real32 )axis_value / 32767.0f;
  } else if( axis_value < 0 ) {
    normalised_value = ( real32 )axis_value / 32768.0f;
  } else {
    normalised_value = 0.0f;
  }
  
  // if( axis_value > 100 || axis_value < -100 ) {
  //   real32 norm = ( real32 )axis_value / 32767.0f;
  //   SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "raw %d = norm %f...norm orig %f\n", axis_value, norm, normalised_value );
  // }
  
  if( event -> jaxis.axis == 0 ) { // left and right
    new_buttons -> joy_axis_x = normalised_value;
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy x axis raw reading %d\n", axis_value );
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy x axis norm reading %f\n", normalised_value );
  } else if( event -> jaxis.axis == 1 ) { // up and down
    new_buttons -> joy_axis_y = normalised_value;
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy y axis raw reading %d\n", axis_value );
    // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy y axis norm reading %f\n", normalised_value );
  }
  
}

void handle_sdl_input_event( SDL_Event* event, Buttons_pressed* old_buttons, Buttons_pressed* new_buttons ) {
  
  switch( event -> type ) {
    
    case SDL_JOYAXISMOTION : {
      on_joy_axis_move( event, new_buttons );
    } break;
    
    case SDL_QUIT : {
      new_buttons -> quit = true;
    } break;
    
    case SDL_KEYDOWN : {
      if( event -> key.repeat == 0 )
        input_on_key_down( event, old_buttons, new_buttons );
    } break;
    
    case SDL_KEYUP : {
      input_on_key_up( event, old_buttons, new_buttons );
    } break;
    
    default : break;
  }
}


#endif //INPUT_HPP
