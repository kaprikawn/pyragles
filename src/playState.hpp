#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>
#include "gameState.hpp"
#include "entity.hpp"
#include "components/physicsComponent.hpp"
#include "rendering/renderer.hpp"

class PlayState : public GameState {
  private:
    
    int               nextLevel_;
    Renderer          renderer_;
    
    Entity            ship_;
    PhysicsComponent  shipPhysics_;
    
    
    static const std::string  s_playID;
    
  public:
    
    bool  onEnter( int levelNumber );
    void  update( GLfloat dt );
    void  render();
    bool  onExit();
    int   nextLevel();
    
    std::string getStateID() const { return s_playID; }

};

#endif //PLAYSTATE_HPP