#ifndef INPUT_HPP
#define INPUT_HPP

#include "types.hpp"
#include "sdl.hpp"

struct ButtonsPressed {
  bool32 quit       = false;
  bool32 w          = false;
  bool32 a          = false;
  bool32 s          = false;
  bool32 d          = false;
  bool32 arrow_up   = false;
  bool32 arrow_down = false;
  bool32 pg_up      = false;
  bool32 pg_down    = false;
  f32 joy_axis_x = 0.0f;
  f32 joy_axis_y = 0.0f;
};

struct GameInput {
  f32 joy_axis_x = 0.0f;
  f32 joy_axis_y = 0.0f;
  
  bool quit         = false;
  
  // bool w_pressed    = false;
  // bool w_held       = false;
  // bool w_released   = false;
  // bool a_pressed    = false;
  // bool a_held       = false;
  // bool a_released   = false;
  // bool s_pressed    = false;
  // bool s_held       = false;
  // bool s_released   = false;
  // bool d_pressed    = false;
  // bool d_held       = false;
  // bool d_released   = false;
  bool arrow_up_pressed     = false;
  bool arrow_up_held        = false;
  bool arrow_up_released    = false;
  bool arrow_down_pressed   = false;
  bool arrow_down_held      = false;
  bool arrow_down_released  = false;
  bool pg_up_pressed        = false;
  bool pg_up_held           = false;
  bool pg_up_released       = false;
  bool pg_down_pressed      = false;
  bool pg_down_held         = false;
  bool pg_down_released     = false;
};

void reset_game_inputs_pressed( ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  old_buttons -> w          = new_buttons -> w;
  old_buttons -> a          = new_buttons -> a;
  old_buttons -> s          = new_buttons -> s;
  old_buttons -> d          = new_buttons -> d;
  old_buttons -> arrow_up   = new_buttons -> arrow_up;
  old_buttons -> arrow_down = new_buttons -> arrow_down;
}

GameInput get_game_input_state( ButtonsPressed old_buttons, ButtonsPressed new_buttons, bool32 invert_y ) {
  
  GameInput result;
  
  if( old_buttons.quit || new_buttons.quit )
    result.quit = true;
  
  // if( !old_buttons.w && new_buttons.w ) {
  //   result.w_pressed  = true;
  //   result.w_held     = true;
  // } else if( old_buttons.w && new_buttons.w ) {
  //   result.w_held     = true;
  // } else if( old_buttons.w && !new_buttons.w ) {
  //   result.w_released = true;
  // }
  
  // if( !old_buttons.a && new_buttons.a ) {
  //   result.a_pressed  = true;
  //   result.a_held     = true;
  // } else if( old_buttons.a && new_buttons.a ) {
  //   result.a_held     = true;
  // } else if( old_buttons.a && !new_buttons.a ) {
  //   result.a_released = true;
  // }
  
  // if( !old_buttons.s && new_buttons.s ) {
  //   result.s_pressed  = true;
  //   result.s_held     = true;
  // } else if( old_buttons.s && new_buttons.s ) {
  //   result.s_held     = true;
  // } else if( old_buttons.s && !new_buttons.s ) {
  //   result.s_released = true;
  // }
  
  // if( !old_buttons.d && new_buttons.d ) {
  //   result.d_pressed  = true;
  //   result.d_held     = true;
  // } else if( old_buttons.d && new_buttons.d ) {
  //   result.d_held     = true;
  // } else if( old_buttons.d && !new_buttons.d ) {
  //   result.d_released = true;
  // }
  
  if( !old_buttons.arrow_up && new_buttons.arrow_up ) {
    result.arrow_up_pressed  = true;
    result.arrow_up_held     = true;
  } else if( old_buttons.arrow_up && new_buttons.arrow_up ) {
    result.arrow_up_held     = true;
  } else if( old_buttons.arrow_up && !new_buttons.arrow_up ) {
    result.arrow_up_released = true;
  }
  
  if( !old_buttons.arrow_down && new_buttons.arrow_down ) {
    result.arrow_down_pressed  = true;
    result.arrow_down_held     = true;
  } else if( old_buttons.arrow_down && new_buttons.arrow_down ) {
    result.arrow_down_held     = true;
  } else if( old_buttons.arrow_down && !new_buttons.arrow_down ) {
    result.arrow_down_released = true;
  }
  
  if( !old_buttons.pg_up && new_buttons.pg_up ) {
    result.pg_up_pressed  = true;
    result.pg_up_held     = true;
  } else if( old_buttons.pg_up && new_buttons.pg_up ) {
    result.pg_up_held     = true;
  } else if( old_buttons.pg_up && !new_buttons.pg_up ) {
    result.pg_up_released = true;
  }
  
  if( !old_buttons.pg_down && new_buttons.pg_down ) {
    result.pg_down_pressed  = true;
    result.pg_down_held     = true;
  } else if( old_buttons.pg_down && new_buttons.pg_down ) {
    result.pg_down_held     = true;
  } else if( old_buttons.pg_down && !new_buttons.pg_down ) {
    result.pg_down_released = true;
  }
  
  f32 joy_axis_x = new_buttons.joy_axis_x;
  f32 joy_axis_y = new_buttons.joy_axis_y;
  
  if( joy_axis_x == 0.0f ) {
    if( new_buttons.d ) {
      joy_axis_x = 1.0f;
    } else if( new_buttons.a ) {
      joy_axis_x = -1.0f;
    }
  }
  if( joy_axis_y == 0.0f ) {
    if( new_buttons.s ) {
      joy_axis_y = 1.0f;
    } else if( new_buttons.w ) {
      joy_axis_y = -1.0f;
    }
  }
    
  if( invert_y ) {
    joy_axis_y = -joy_axis_y;
  }
  
  result.joy_axis_x = joy_axis_x;
  result.joy_axis_y = joy_axis_y;
  
  return result;
}

void input_on_key_down( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  if( event -> key.repeat > 0 )
    return;
  
  u32 scancode = event -> key.keysym.scancode;
  
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
    
    case SDL_SCANCODE_UP : {
      new_buttons -> arrow_up = true;
      old_buttons -> arrow_up = false;
    } break;
    
    case SDL_SCANCODE_DOWN : {
      new_buttons -> arrow_down = true;
      old_buttons -> arrow_down = false;
    } break;
    
    case SDL_SCANCODE_PAGEUP : {
      new_buttons -> pg_up = true;
      old_buttons -> pg_up = false;
    } break;
    
    case SDL_SCANCODE_PAGEDOWN : {
      new_buttons -> pg_down = true;
      old_buttons -> pg_down = false;
    } break;
    
    default : break;
  }
}

void input_on_key_up( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  u32 scancode = event -> key.keysym.scancode;
  
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
    
    case SDL_SCANCODE_UP : {
      new_buttons -> arrow_up = false;
      old_buttons -> arrow_up = true;
    } break;
    
    case SDL_SCANCODE_DOWN : {
      new_buttons -> arrow_down = false;
      old_buttons -> arrow_down = true;
    } break;
    
    case SDL_SCANCODE_PAGEUP : {
      new_buttons -> pg_up = false;
      old_buttons -> pg_up = true;
    } break;
    
    case SDL_SCANCODE_PAGEDOWN : {
      new_buttons -> pg_down = false;
      old_buttons -> pg_down = true;
    } break;
    
    default : break;
  }
}

u32 gamepad_count          = 0;
const u32 MAX_CONTROLLERS  = 4;
SDL_Joystick* gamepads[ MAX_CONTROLLERS ];

void initialise_gamepads() {
  
  gamepad_count = SDL_NumJoysticks();
  
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Detected %d attached gamepads\n", gamepad_count );
  
  if( gamepad_count == 0 ) return;
  
  SDL_Joystick* gamepad = NULL;
  
  for( u32 i = 0; i < gamepad_count && i < MAX_CONTROLLERS; i++ ) {
    gamepad = SDL_JoystickOpen( i );
    if( gamepad ) {
      gamepads[ i ] = gamepad;
    } else {
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Could not open gamecontroller %d, %s\n", i, SDL_GetError());
    }
  }
}

s16 deadzone = 100;

void on_joy_axis_move( SDL_Event* event, ButtonsPressed* new_buttons ) {
  
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "HERE\n" );
  
  s16   axis_value  = event -> jaxis.value;
  f32  normalised_value;
  
  if( axis_value < deadzone && axis_value > -deadzone ) {
    normalised_value = 0.0f;
  } else if( axis_value > 0 ) {
    normalised_value = ( f32 )axis_value / 32767.0f;
  } else if( axis_value < 0 ) {
    normalised_value = ( f32 )axis_value / 32768.0f;
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

void handle_sdl_input_event( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
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
