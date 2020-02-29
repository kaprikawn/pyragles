#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SDL2/SDL.h>
#include <GLES2/gl2.h>
#include <vector>

enum Buttons {
    LEFT, RIGHT, ASCEND, DESCEND, FIRE
  , MAX_BUTTON_NUM
  , MAX_BUTTON = MAX_BUTTON_NUM
};

class InputHandler {
  
  private:
    
    Uint32 lastFrameTime_ = 0;
    
    GLfloat joyAxisX_ = 0.0f;
    GLfloat joyAxisY_ = 0.0f;
    
    bool justPressed_[ MAX_BUTTON ];
  
    // keyboard
    const Uint8*  keystates_ = SDL_GetKeyboardState( NULL );
    void          onKeyDown( SDL_Event& event );
    void          onKeyUp( SDL_Event& event );
    
    // gamepad
    bool    gamepadsInitialised_ = false;
    int     whichOne_;
    void    onJoystickAxisMove( SDL_Event &event );
    
    bool    fireJustPressed_  = false;
    bool    yInverted_        = false;
    
    Uint8   currentHat_   = 0;
    
    std::vector<SDL_Joystick*>  gamepads_;
    
    bool quit_ = false;
    
  public:
  
    InputHandler( bool invertY );
    ~InputHandler();
    
    void processEvent( SDL_Event& event, Uint32 frameTime );
    void reset();
    
    void calculateJoyAxis();
    
    bool isPressed( int button );
    bool justPressed( int button ) {
      if( justPressed_[ button ] ) {
        justPressed_[ button ] = false;
        return true;
      }
      return false;
    }
    
    float joyAxisX() { return joyAxisX_; }
    float joyAxisY() {
      if( yInverted_ )
        return -joyAxisY_;
      
      return joyAxisY_;
    }
    
    // gamepad
    void  initialiseGamepads();
    bool  gamepadsInitialised() { return gamepadsInitialised_; }
    //bool  getButtonState( int buttonNumber ) { return buttonStates_[buttonNumber]; }
    void  onHatMotion( SDL_Event &event );
    void  onGamepadButtonDown( SDL_Event &event );
    
    bool quit() { return quit_; }
    
    bool yInverted() { return yInverted_; }
    
};

#endif //INPUTHANDLER_HPP