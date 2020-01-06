#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "inputHandler.hpp"
#include "gameObject.hpp"
#include "../deps/json.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    std::shared_ptr<Camera>     camera_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    nlohmann::json              levelJson_;
    
    int                         nextLevel_;
    
    std::shared_ptr<gameObject> myBox_;
    
  public:
    
    virtual bool onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    virtual int  nextLevel();
    
    void addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading );
    void loadMeshes();
    
    virtual std::string getStateID() const { return s_playID; }
    
    nlohmann::json json( int levelNumber );

};

#endif //PLAYSTATE_HPP