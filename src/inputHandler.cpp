#include "inputHandler.hpp"
#include <iostream>

InputHandler::InputHandler( bool invertY ) {
  if( invertY )
    yInverted_ = true;
}

void InputHandler::calculateJoyAxis() {
  
  if( keystates_[ SDL_SCANCODE_W ] == 1 ) {
    joyAxisY_ = 1.0f;
  } else if( keystates_[ SDL_SCANCODE_S ] == 1 ) {
    joyAxisY_ = -1.0f;
  } else {
    joyAxisY_ = 0.0f;
  }
  
  if( keystates_[ SDL_SCANCODE_A ] == 1 ) {
    joyAxisX_ = -1.0f;
  } else if( keystates_[ SDL_SCANCODE_D ] == 1 ) {
    joyAxisX_ = 1.0f;
  } else {
    joyAxisX_ = 0.0f;
  }
}

void InputHandler::onKeyDown( SDL_Event& event ) {
  
  keystates_ = SDL_GetKeyboardState( NULL );
  
  if(  event.key.keysym.scancode == SDL_SCANCODE_W
    || event.key.keysym.scancode == SDL_SCANCODE_A
    || event.key.keysym.scancode == SDL_SCANCODE_S
    || event.key.keysym.scancode == SDL_SCANCODE_D
  )
    calculateJoyAxis();
    
  if( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
    justPressed_[ FIRE ] = true;
  }
  
  if( keystates_[ SDL_SCANCODE_ESCAPE ] == 1 ) {
    quit_ = true;
  }
}

void InputHandler::onKeyUp( SDL_Event& event ) {
  keystates_ = SDL_GetKeyboardState( NULL );
  
  if(  event.key.keysym.scancode == SDL_SCANCODE_W
    || event.key.keysym.scancode == SDL_SCANCODE_A
    || event.key.keysym.scancode == SDL_SCANCODE_S
    || event.key.keysym.scancode == SDL_SCANCODE_D
  )
    calculateJoyAxis();
}

bool InputHandler::isPressed( int button ) {
  
  return false;
}

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
        for( int b = 0; b < SDL_JoystickNumButtons( joy ); b++ ) {
          //buttonStates_.push_back( false );
          //padJustPressed_.push_back( false );
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

void InputHandler::onHatMotion( SDL_Event &event ) {
  
  for( unsigned int i = 0; i < gamepads_.size(); i++ )
    currentHat_ = SDL_JoystickGetHat( gamepads_[i], 0 );
}

void InputHandler::onGamepadButtonDown( SDL_Event &event ) {
  if( event.jbutton.button == 2 )
    justPressed_[ FIRE ] = true;
}

void InputHandler::onJoystickAxisMove( SDL_Event &event ) {
  
  Sint16  joyAxisX  = 0;
  Sint16  joyAxisY  = 0;
  
  static const int joystickDeadZone = 3000;
  
  if( event.jaxis.axis == 0 ) {
    if( event.jaxis.value > joystickDeadZone || event.jaxis.value < -joystickDeadZone ) {
      joyAxisX = event.jaxis.value;
    }
    
    if( joyAxisX > 0 ) { // right on the stick
      joyAxisX_ = ( float ) joyAxisX / 32767.0f;
    } else if( joyAxisX < 0 ) { // left on the stick
      joyAxisX_ = ( float ) joyAxisX / 32768.0f;
    } else {
      joyAxisX_ = 0.0f;
    }    
    
  } else if( event.jaxis.axis == 1 ) {
    if( event.jaxis.value > joystickDeadZone || event.jaxis.value < -joystickDeadZone ) {
      joyAxisY = event.jaxis.value; // swap = position up, negative down
    }
    
    if( joyAxisY > 0 ) { // down on the stick
      joyAxisY_ = -( joyAxisY / 32767.0f );
    } else if( joyAxisY < 0 ) { // up on the stick
      joyAxisY_ = -joyAxisY / 32768.0f;
    } else {
      joyAxisY_ = 0.0f;
    }
  }
}

void InputHandler::processEvent( SDL_Event& event, Uint32 frameTime ) {
    
  switch( event.type ) {
    
    // gamepad
    case SDL_JOYAXISMOTION:
      onJoystickAxisMove( event );    break;
    case SDL_JOYBUTTONDOWN:
      onGamepadButtonDown( event );   break;
      
    // keyboard
    case SDL_KEYDOWN:
      if( event.key.repeat == 0 ) {
        onKeyDown( event ); break;
      }
      break;
    case SDL_KEYUP:
      onKeyUp( event );   break;
    
    case SDL_QUIT:
        quit_ = true;     break;
      
      default:
        break;
  }
  
}

void InputHandler::reset() {
  justPressed_[ FIRE ] = false;
}

InputHandler::~InputHandler() {
  
}
