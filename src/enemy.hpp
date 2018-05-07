#ifndef ENEMEY_HPP
#define ENEMEY_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"

class Enemy : public PhysicsObject {
  private:
    
    std::shared_ptr<InputHandler> inputHandler_;
    //std::shared_ptr<glm::vec3>    shipPosition_;
    
    GLfloat yAngle_ = 0.0f;
    
  public:
    Enemy( PhysicsObjectParams physicsObjectParams );
    ~Enemy(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    void  calculateRotation( GLfloat dt );
    void registerCollision( CollisionData collisionData, CollisionProperties collisionProperties );
    CollisionProperties collisionProperties();
};

#endif // ENEMEY_HPP