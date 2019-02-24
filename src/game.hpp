#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <memory>
#include "inputHandler.hpp"
#include "gameStateMachine.hpp"
#include "camera.hpp"

class Game {
  private:
    SDL_Window*     window_;
    SDL_GLContext   glContext_;
    
    std::shared_ptr<InputHandler>     inputHandler_;
    std::unique_ptr<GameStateMachine> gameStateMachine_;
    std::shared_ptr<Camera>           camera_;
    
    int             newState_ = -1;
    bool            running_  = false;
    int             transitionType_;
    
  public:
    
    Game( bool fullscreen );
    ~Game(){}
    
    bool init( const char* title, int xpos, int ypos, int windowWidth, int windowHeight, int flags );
    
    void run();
    void render();
    
    void setNewState( int newState, int transitionType );
    void changeGameState( int newState, int transitionType );
};

#endif //GAME_HPP