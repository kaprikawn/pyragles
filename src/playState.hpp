#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>
#include "SDL.h"
#include "gameState.hpp"
#include "structs.hpp"
#include "target.hpp"
#include "hero.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    Uint32      levelStart_;
    Uint32      currentTime_;
    
    std::shared_ptr<Hero>   hero_;
    std::shared_ptr<Target> target_;
    
  public:
    
    virtual bool onEnter();
    virtual void update( float dt );
    virtual void render();
    virtual bool onExit();
    
    void  addGlObject( std::shared_ptr<GlObject> glObject );
    
    virtual std::string getStateID() const { return s_playID; }

    void gameLogic( float dt );
};

#endif //PLAYSTATE_HPP
