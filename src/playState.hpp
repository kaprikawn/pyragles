#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "ship.hpp"
#include "target.hpp"
#include "meshLoader.hpp"
#include "scenary.hpp"
#include "floor.hpp"
#include "collision.hpp"
#include "audio.hpp"
#include "../deps/json.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    std::shared_ptr<Renderer>   renderer_;
    std::shared_ptr<MeshLoader> meshLoader_;
    std::shared_ptr<Camera>     camera_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    std::shared_ptr<glm::vec3>  shipPosition_;
    std::shared_ptr<Ship>       ship_;
    std::shared_ptr<Target>     target_;
    std::shared_ptr<Scenary>    arch_;
    
    Collision                   collision_;
    std::shared_ptr<Audio>      audio_;
    
    unsigned int                nextObjectID_ = 1;
    
    nlohmann::json              levelJson_;
    
    int                         nextLevel_;
    
    
  public:
    
    virtual bool onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    virtual int  nextLevel();
    
    void addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading );
    
    virtual std::string getStateID() const { return s_playID; }
    
    nlohmann::json json( int levelNumber );

};

#endif //PLAYSTATE_HPP