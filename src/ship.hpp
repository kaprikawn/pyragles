#ifndef SHIP_HPP
#define SHIP_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "target.hpp"
#include "timer.hpp"

class Ship : public PhysicsObject {
  private:
    
    std::shared_ptr<InputHandler> inputHandler_;
    std::shared_ptr<glm::vec3>    shipPosition_;
    std::shared_ptr<Target>       target_;
    
    GLfloat   collidedDirection_  = 1.0f;
    GLfloat   zAngle_             = 0.0f;
    Timer     collidedTimer_;
    
  public:
    Ship( PhysicsObjectParams physicsObjectParams, std::shared_ptr<Target> target );
    ~Ship(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    void  calculateRotation( GLfloat dt );
    void registerCollision( CollisionData collisionData, CollisionProperties collisionProperties );
    CollisionProperties collisionProperties();
    
    void handleInput( GLfloat dt );
};

#endif // SHIP_HPP