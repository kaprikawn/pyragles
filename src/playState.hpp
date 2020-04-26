#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>
#include "gameState.hpp"
#include "glCallLog.hpp"
#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "shader.hpp"
#include "ship.hpp"
#include "floor.hpp"
#include "hud.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include "projectileManager.hpp"

class PlayState : public GameState {
  private:
    static const std::string    s_playID;
    
    int                         nextLevel_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    std::unique_ptr<Ship>               ship_;
    std::unique_ptr<Floor>              floor_;
    std::unique_ptr<Hud>                hud_;
    std::unique_ptr<Projectile>         bullet_;
    std::unique_ptr<ProjectileManager>  projectiles_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    
    unsigned short int          bombCount_ = 9;
    
    
  public:
    
    virtual bool onEnter( int levelNumber );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    virtual int  nextLevel();
    
    virtual std::string getStateID() const { return s_playID; }

};

#endif //PLAYSTATE_HPP