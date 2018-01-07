#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <vector>
#include <SDL2/SDL.h>

class InputHandler {
  private:
  
    // keyboard
    const Uint8*  keystates_ = SDL_GetKeyboardState( NULL );
    void          onKeyDown();

    InputHandler  (){}
    ~InputHandler (){}
    static InputHandler* instance_;
  
  public:
    
    void update();
    
    static InputHandler* Instance() {
      if( instance_ == 0 ) {
        instance_ = new InputHandler();
      }
      return instance_;
    }
};

typedef InputHandler TheInputHandler;

#endif //INPUTHANDLER_HPP
