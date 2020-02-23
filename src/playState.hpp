#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "indexBuffer.hpp"
#include "vertexBuffer.hpp"
#include "shader.hpp"
#include "ship.hpp"
#include "floor.hpp"
#include <memory>

class PlayState : public GameState {
  private:
    static const std::string    s_playID;
    
    int                         nextLevel_;
    
    std::shared_ptr<Camera>     camera_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    std::unique_ptr<Ship>       ship_;
    std::unique_ptr<Floor>      floor_;
    
  public:
    
    virtual bool onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    virtual int  nextLevel();
    
    virtual std::string getStateID() const { return s_playID; }

};

#endif //PLAYSTATE_HPP