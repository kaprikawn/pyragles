#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "inputHandler.hpp"
#include "gameObject.hpp"
#include "../deps/json.hpp"
#include "renderer.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    std::shared_ptr<Camera>     camera_;
    std::shared_ptr<Renderer>   renderer_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    nlohmann::json              levelJson_;
    
    int                         nextLevel_;
    
    std::shared_ptr<GameObject> myBox_;
    
  public:
    
    virtual bool onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    virtual int  nextLevel();
    
    virtual std::string getStateID() const { return s_playID; }
    
    nlohmann::json json( int levelNumber );

};

#endif //PLAYSTATE_HPP