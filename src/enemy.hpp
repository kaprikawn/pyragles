#ifndef ENEMEY_HPP
#define ENEMEY_HPP

#include <GLES2/gl2.h>
#include <memory>
#include <vector>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "timer.hpp"

struct NextMovement {
  GLfloat x         = 0.0f;
  GLfloat y         = 0.0f;
  GLfloat z         = 0.0f;
  GLfloat duration  = 10.0f;
};

class Enemy : public PhysicsObject {
  private:
    
    std::shared_ptr<InputHandler> inputHandler_;
    //std::shared_ptr"../deps/glm::vec3>    shipPosition_;
    std::vector<NextMovement>     movements_;
    
    GLfloat yAngle_ = 0.0f;
    Timer   movementTimer_;
    
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