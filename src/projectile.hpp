#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "target.hpp"

class Projectile : public PhysicsObject {
  private:
    
    std::shared_ptr<glm::vec3>    shipPosition_;
    std::shared_ptr<Target>       target_;
    
  public:
    Projectile( PhysicsObjectParams physicsObjectParams, std::shared_ptr<Target> target );
    ~Projectile(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    void  calculateRotation( GLfloat dt );
    
    CollisionProperties collisionProperties();
    
};

#endif // PROJECTILE_HPP