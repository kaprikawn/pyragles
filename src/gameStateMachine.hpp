#ifndef GAMESTATEMACHINE_HPP
#define GAMESTATEMACHINE_HPP

#include <vector>
#include <memory>
#include "gameState.hpp"

class GameStateMachine {
  private:
    std::vector<std::unique_ptr<GameState>> gameStates_;
    
  public:
    void pushState  ( std::unique_ptr<GameState> state );
    void changeState( std::unique_ptr<GameState> state );
    void popState   ();
    
    void update     ( float dt );
    void render     ();
    
    std::string     getCurrentGameStateID();
};

#endif //GAMESTATEMACHINE_HPP
