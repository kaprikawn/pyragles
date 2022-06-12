#ifndef INPUT_HPP
#define INPUT_HPP

#include "types.hpp"
#include "sdl.hpp"

struct ButtonsPressed {
  bool32  quit        = false;
  bool32  w           = false;
  bool32  a           = false;
  bool32  s           = false;
  bool32  d           = false;
  bool32  arrow_up    = false;
  bool32  arrow_down  = false;
  bool32  arrow_left  = false;
  bool32  arrow_right = false;
  bool32  pg_up       = false;
  bool32  pg_down     = false;
  bool32  dpad_up     = false;
  bool32  dpad_down   = false;
  bool32  dpad_left   = false;
  bool32  dpad_right  = false;
  bool32  btn_left    = false; // e.g. square on ps controller
  bool32  btn_right   = false; // e.g. circle on ps controller
  bool32  btn_up      = false; // e.g. triangle on ps controller
  bool32  btn_down    = false; // e.g. X on ps controller
  f32     joy_axis_x  = 0.0f;
  f32     joy_axis_y  = 0.0f;
};

struct GameInput {
  bool32  quit                = false;
  bool32  btn_left_pressed    = false;
  bool32  btn_right_pressed   = false;
  bool32  btn_top_pressed     = false;
  bool32  btn_bottom_pressed  = false;
  bool32  btn_ltrig_pressed   = false;
  bool32  btn_rtrig_pressed   = false;
  f32     joy_axis_x          = 0.0f;
  f32     joy_axis_y          = 0.0f;
};

void reset_game_inputs_pressed( ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  old_buttons -> w          = new_buttons -> w;
  old_buttons -> a          = new_buttons -> a;
  old_buttons -> s          = new_buttons -> s;
  old_buttons -> d          = new_buttons -> d;
  old_buttons -> arrow_up   = new_buttons -> arrow_up;
  old_buttons -> arrow_down = new_buttons -> arrow_down;
  old_buttons -> pg_up      = new_buttons -> pg_up;
  old_buttons -> pg_down    = new_buttons -> pg_down;
  old_buttons -> dpad_up    = new_buttons -> dpad_up;
  old_buttons -> dpad_down  = new_buttons -> dpad_down;
  old_buttons -> dpad_left  = new_buttons -> dpad_left;
  old_buttons -> dpad_right = new_buttons -> dpad_right;
  old_buttons -> btn_left   = new_buttons -> btn_left;
  old_buttons -> btn_right  = new_buttons -> btn_right;
  old_buttons -> btn_up     = new_buttons -> btn_up;
  old_buttons -> btn_down   = new_buttons -> btn_down;
}

GameInput get_game_input_state( ButtonsPressed old_buttons, ButtonsPressed new_buttons, bool32 invert_y ) {
  
  GameInput result;
  
  if( old_buttons.quit || new_buttons.quit )
    result.quit = true;
  
  if( new_buttons.w || new_buttons.arrow_up || new_buttons.dpad_up ) {
    result.joy_axis_y = -1.0f;
  } else if( new_buttons.s || new_buttons.arrow_down || new_buttons.dpad_down ) {
    result.joy_axis_y =  1.0f;
  } else {
    result.joy_axis_y =  new_buttons.joy_axis_y;
  }
  
  if( new_buttons.a || new_buttons.arrow_left || new_buttons.dpad_left ) {
    result.joy_axis_x = -1.0f;
  } else if( new_buttons.d || new_buttons.arrow_right || new_buttons.dpad_right ) {
    result.joy_axis_x =  1.0f;
  } else {
    result.joy_axis_x =  new_buttons.joy_axis_x;
  }
  
  if( invert_y ) {
    f32 joy_axis_y = result.joy_axis_y;
    joy_axis_y = -joy_axis_y;
    result.joy_axis_y = joy_axis_y;
  }
  
  if( !old_buttons.btn_left && new_buttons.btn_left ) {
    result.btn_left_pressed = true;
#ifdef INPUT_DEBUG
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "fire button pressed\n" );
#endif
  }
  
  return result;
}

void input_on_key_down( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  if( event -> key.repeat > 0 )
    return;
  
  u32 scancode = event -> key.keysym.scancode;
  
#ifdef INPUT_DEBUG
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "input_on_key_down %d\n", scancode );
#endif
  
  switch( scancode ) {
    
    case SDL_SCANCODE_ESCAPE    : { new_buttons -> quit       = true; } break;
    case SDL_SCANCODE_W         : { new_buttons -> w          = true; } break;
    case SDL_SCANCODE_A         : { new_buttons -> a          = true; } break;
    case SDL_SCANCODE_S         : { new_buttons -> s          = true; } break;
    case SDL_SCANCODE_D         : { new_buttons -> d          = true; } break;
    case SDL_SCANCODE_UP        : { new_buttons -> dpad_up    = true; } break;
    case SDL_SCANCODE_DOWN      : { new_buttons -> dpad_down  = true; } break;
    case SDL_SCANCODE_LEFT      : { new_buttons -> dpad_left  = true; } break;
    case SDL_SCANCODE_RIGHT     : { new_buttons -> dpad_right = true; } break;
    
    default : break;
  }
}

void input_on_key_up( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  u32 scancode = event -> key.keysym.scancode;
  
  switch( scancode ) {
    
    case SDL_SCANCODE_W         : { new_buttons -> w          = false; } break;
    case SDL_SCANCODE_A         : { new_buttons -> a          = false; } break;
    case SDL_SCANCODE_S         : { new_buttons -> s          = false; } break;
    case SDL_SCANCODE_D         : { new_buttons -> d          = false; } break;
    case SDL_SCANCODE_UP        : { new_buttons -> dpad_up    = false; } break;
    case SDL_SCANCODE_DOWN      : { new_buttons -> dpad_down  = false; } break;
    case SDL_SCANCODE_LEFT      : { new_buttons -> dpad_left  = false; } break;
    case SDL_SCANCODE_RIGHT     : { new_buttons -> dpad_right = false; } break;
    
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

void on_hat_motion( SDL_Event* event, ButtonsPressed* new_buttons ) {

#ifdef INPUT_DEBUG
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "hat motion detected\n" );
#endif
  
}

void on_joy_button_down( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  u8 button_index = event -> jbutton.button;
  
#ifdef INPUT_DEBUG
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "on_joy_button_down %d pressed\n", button_index );
#endif
  
  switch( button_index ) {
    // wasd
    case 11 : { new_buttons -> dpad_up    = true; } break;
    case 12 : { new_buttons -> dpad_down  = true; } break;
    case 13 : { new_buttons -> dpad_left  = true; } break;
    case 14 : { new_buttons -> dpad_right = true; } break;
    // arrow keys, is the dpad on the pyra
    case 82 : { new_buttons -> dpad_up    = true; } break;
    case 81 : { new_buttons -> dpad_down  = true; } break;
    case 80 : { new_buttons -> dpad_left  = true; } break;
    case 79 : { new_buttons -> dpad_right = true; SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "right pressed %d pressed\n", button_index ); } break;
    
    default : break;
  }
}

void on_joy_button_up( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  u8 button_index = event -> jbutton.button;
  
  switch( button_index ) {
    case 11 : { new_buttons -> dpad_up    = false; } break;
    case 12 : { new_buttons -> dpad_down  = false; } break;
    case 13 : { new_buttons -> dpad_left  = false; } break;
    case 14 : { new_buttons -> dpad_right = false; } break;
    case 82 : { new_buttons -> dpad_up    = false; } break;
    case 81 : { new_buttons -> dpad_down  = false; } break;
    case 80 : { new_buttons -> dpad_left  = false; } break;
    case 79 : { new_buttons -> dpad_right = false; } break;
    default : break;
  }
}

void on_joy_axis_move( SDL_Event* event, ButtonsPressed* new_buttons ) {
  
  s16 axis_value  = event -> jaxis.value;
  f32 normalised_value;
  
  if( axis_value < deadzone && axis_value > -deadzone ) {
    normalised_value = 0.0f;
  } else if( axis_value > 0 ) {
    normalised_value = ( f32 ) axis_value / 32767.0f;
  } else if( axis_value < 0 ) {
    normalised_value = ( f32 ) axis_value / 32768.0f;
  } else {
    normalised_value = 0.0f;
  }
  
  if( event -> jaxis.axis == 0 ) { // left and right
    new_buttons -> joy_axis_x = normalised_value;
    
  } else if( event -> jaxis.axis == 1 ) { // up and down
    new_buttons -> joy_axis_y = normalised_value;
  }
  
#ifdef INPUT_DEBUG
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy x axis raw reading %d\n", axis_value );
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy x axis norm reading %f\n", normalised_value );
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy y axis raw reading %d\n", axis_value );
  SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "joy y axis norm reading %f\n", normalised_value );
#endif
  
}

void handle_sdl_input_event( SDL_Event* event, ButtonsPressed* old_buttons, ButtonsPressed* new_buttons ) {
  
  // SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "type is %d\n", event -> type );
  
  switch( event -> type ) {
    
    case SDL_JOYHATMOTION : {
      on_hat_motion( event, new_buttons );
    } break;
    
    case SDL_JOYBUTTONDOWN  : {
      on_joy_button_down( event, old_buttons, new_buttons );
    } break;
    
    case SDL_JOYBUTTONUP  : {
      on_joy_button_up( event, old_buttons, new_buttons );
    } break;
    
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
