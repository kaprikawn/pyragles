#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "ship.hpp"
#include "target.hpp"
#include "meshLoader.hpp"
#include "scenary.hpp"
#include "collision.hpp"
#include "floor.hpp"

class PlayState : public GameState {
  private:
    static const std::string  s_playID;
    
    std::shared_ptr<Renderer>   renderer_;
    std::shared_ptr<MeshLoader> meshLoader_;
    std::unique_ptr<Collision>  collision_;
    
    glm::mat4                   viewProjectionMatrix_;
    
    std::shared_ptr<glm::vec3>  shipPosition_;
    std::shared_ptr<Ship>       ship_;
    std::shared_ptr<Target>     target_;
    std::shared_ptr<Scenary>    arch_;
    
    
  public:
    
    virtual bool onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera );
    virtual void update( GLfloat dt );
    virtual void render();
    virtual bool onExit();
    
    void addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading );
    
    virtual std::string getStateID() const { return s_playID; }

};

#endif //PLAYSTATE_HPP