#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <vector>
#include <SDL2/SDL.h>
#include "vector.hpp"

class InputHandler {
  private:
  
    // keyboard
    const Uint8*  keystates_ = SDL_GetKeyboardState( NULL );
    void          onKeyDown();
    void          onKeyUp();
    
    // gamepad
    bool    gamepadsInitialised_ = false;
    int     whichOne_;
    Uint8   currentHat_ = 0;
    void    onJoystickAxisMove( SDL_Event &event );
    static const int joystickDeadZone_ = 3000;
    Sint16  joyAxisX_   = 0;
    Sint16  joyAxisY_   = 0;
    
    std::vector<SDL_Joystick*>  gamepads_;
    std::vector<bool>           buttonStates_;
    
    InputHandler  (){}
    ~InputHandler (){}
    static InputHandler* instance_;
  
  public:
    
    // gamepad
    void  initialiseGamepads();
    bool  gamepadsInitialised() { return gamepadsInitialised_; }
    bool  getButtonState( int buttonNumber ) { return buttonStates_[buttonNumber]; }
    void  onHatMotion( SDL_Event &event );
    GLfloat joyAxisX();
    GLfloat joyAxisY();
    
    // keyboard
    bool  isKeyDown( SDL_Scancode key );
    
    bool  isPressed( int button );
    
    void update();
    void clean();
    
    static InputHandler* Instance() {
      if( instance_ == 0 ) {
        instance_ = new InputHandler();
      }
      return instance_;
    }
};

typedef InputHandler TheInputHandler;

#endif //INPUTHANDLER_HPP
