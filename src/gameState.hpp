#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <memory>
#include "SDL.h"

class GameState {
  protected:
    GameState() {}
    
  public:
    virtual ~GameState() {}
    
    virtual bool onEnter() = 0;
    virtual void update ( float dt ) = 0;
    virtual void render () = 0;
    virtual bool onExit () = 0;
    virtual std::string getStateID() const = 0;
};

#endif //GAMESTATE_HPP
