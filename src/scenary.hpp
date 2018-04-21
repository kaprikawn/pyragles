#ifndef SCENARY_HPP
#define SCENARY_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"

class Scenary : public PhysicsObject {
  private:
    
  public:
    Scenary( PhysicsObjectParams physicsObjectParams );
    ~Scenary(){}
    
    void update( GLfloat dt, bool skipMove = false );
    void render( glm::mat4 viewProjectionMatrix );
    void clean();
    void registerCollision( CollisionData collisionData, CollisionProperties collisionProperties );
    CollisionProperties collisionProperties();
    
    
    unsigned int stuff() {
      return objectID_;
    }
    
};

#endif // SCENARY_HPP