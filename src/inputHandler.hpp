#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <vector>
#include <SDL2/SDL.h>

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
