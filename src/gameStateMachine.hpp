#ifndef GAMESTATEMACHINE_HPP
#define GAMESTATEMACHINE_HPP

#include <vector>
#include <memory>
#include <GLES2/gl2.h>
#include "gameState.hpp"
#include "playState.hpp"
#include "inputHandler.hpp"

enum GameStates {
  TRANSITION, PLAY, MENU
};

enum TransitionType {
  LOADLEVEL, GAMEOVER, SPLASH
};

class GameStateMachine {
  private:
    std::vector<std::unique_ptr<GameState>> gameStates_;
    
    int nextLevel_ = 11;
    
  public:
    bool pushState  ( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler );
    bool changeState( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler );
    void popState   ();
    
    void update     ( GLfloat dt );
    void render     ();
    
    std::string     getCurrentGameStateID();
};

#endif //GAMESTATEMACHINE_HPP