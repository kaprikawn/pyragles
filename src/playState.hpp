#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <memory>
#include <glm/glm.hpp>
#include "SDL.h"
#include "gameState.hpp"
#include "structs.hpp"
#include "target.hpp"
#include "hero.hpp"
#include "glBuffer.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    GLuint      programID_;
    
    Uint32      levelStart_;
    Uint32      currentTime_;
    
    std::shared_ptr<Hero>       hero_;
    std::shared_ptr<Target>     target_;
    std::shared_ptr<glm::vec3>  heroPosition_;
    
    std::unique_ptr<GlBuffer>   glBuffer_;
    
    GLsizeiptr  vertexBufferSize_     = 0;
    GLsizeiptr  indexBufferSize_      = 0;
    
  public:
    
    virtual bool onEnter();
    virtual void update( float dt );
    virtual void render();
    virtual bool onExit();
    
    void  addGlObject( std::shared_ptr<GlObject> glObject, bool init, bool isLoading );
    
    virtual std::string getStateID() const { return s_playID; }

    void gameLogic( float dt );
};

#endif //PLAYSTATE_HPP
