#include <iostream>
#include <vector>
#include "inputHandler.hpp"
#include "game.hpp"

InputHandler* InputHandler::instance_ = 0;

void InputHandler::initialiseGamepads() {
  if( SDL_WasInit( SDL_INIT_JOYSTICK ) == 0 ) {
    SDL_InitSubSystem( SDL_INIT_JOYSTICK );
  }
  
  if( SDL_NumJoysticks() > 0 ) {
    for( int i = 0; i < SDL_NumJoysticks(); i++ ) {
      SDL_Joystick* joy = SDL_JoystickOpen( i );
      if( joy == NULL ) {
        std::cout << SDL_GetError() << std::endl;
      } else {
        gamepads_.push_back( joy );
        //m_gamepadValues.push_back( std::make_pair( new Vector2D( 0, 0 ), new Vector2D( 0, 0 ) ) );
        for( int b = 0; b < SDL_JoystickNumButtons( joy ); b++ ) {
          buttonStates_.push_back( false );
        }
      }
    }
    
    SDL_JoystickEventState( SDL_ENABLE );
    gamepadsInitialised_ = true;
  } else {
    gamepadsInitialised_ = false;
  }
  std::cout << "Initialised " << gamepads_.size()  << " gamepad(s)" << std::endl;
}

void InputHandler::onKeyDown() {
  keystates_ = SDL_GetKeyboardState( NULL );
  
  if( keystates_[ SDL_SCANCODE_ESCAPE ] == 1 ) {
    TheGame::Instance() -> quit();
  }
}

void InputHandler::onKeyUp() {
  keystates_ = SDL_GetKeyboardState( NULL );
}

bool InputHandler::isKeyDown( SDL_Scancode key ) {
  if( keystates_ != 0 ) {
    if( keystates_[key] == 1 ) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

bool InputHandler::isPressed( int button ) {
  if( button == 0 ) {
    if( keystates_[ SDL_SCANCODE_RIGHT ] == 1 || keystates_[ SDL_SCANCODE_L ] == 1 || currentHat_ == SDL_HAT_RIGHT || currentHat_ == SDL_HAT_RIGHTUP || currentHat_ == SDL_HAT_RIGHTDOWN ) {
      return true;
    }
  } else if( button == 1 ) {
    if( keystates_[ SDL_SCANCODE_LEFT ] == 1 || keystates_[ SDL_SCANCODE_H ] == 1 || currentHat_ == SDL_HAT_LEFT || currentHat_ == SDL_HAT_LEFTUP || currentHat_ == SDL_HAT_LEFTDOWN ) {
      return true;
    }
  } else if( button == 2 ) {
    if( keystates_[ SDL_SCANCODE_UP ] == 1 || keystates_[ SDL_SCANCODE_K ] == 1 || currentHat_ == SDL_HAT_UP || currentHat_ == SDL_HAT_LEFTUP || currentHat_ == SDL_HAT_RIGHTUP ) {
      return true;
    }
  } else if( button == 3 ) {
    if( keystates_[ SDL_SCANCODE_DOWN ] == 1 || keystates_[ SDL_SCANCODE_J ] == 1 || currentHat_ == SDL_HAT_DOWN || currentHat_ == SDL_HAT_LEFTDOWN || currentHat_ == SDL_HAT_RIGHTDOWN ) {
      return true;
    }
  }
  
  return false;
}

void InputHandler::onHatMotion( SDL_Event &event ) {
  
  if( !gamepadsInitialised_ ) {
    return;
  }
  
  for( unsigned int i = 0; i < gamepads_.size(); i++ ) {
    currentHat_ = SDL_JoystickGetHat( gamepads_[i], 0 );
  }
}

void InputHandler::update() {
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
      case SDL_QUIT:
        TheGame::Instance() -> quit(); break;
      case SDL_KEYDOWN:
        onKeyDown(); break;
      case SDL_KEYUP:
        onKeyUp(); break;
      case SDL_JOYHATMOTION:
        onHatMotion( event ); break;
      case SDL_JOYBUTTONDOWN:
        whichOne_ = event.jaxis.which;
        buttonStates_[ event.jbutton.button ] = true;
        break;
      case SDL_JOYBUTTONUP:
        whichOne_ = event.jaxis.which;
        buttonStates_[ event.jbutton.button ] = false;
        break;
      default:
        break;
    }
  }
}

void InputHandler::clean() {
  if( gamepadsInitialised_ ) {
    for( int i = 0; i < SDL_NumJoysticks(); i++ ) {
      SDL_JoystickClose( gamepads_[i] );
    }
  }
}

