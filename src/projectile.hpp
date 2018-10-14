#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "target.hpp"

class Projectile : public PhysicsObject {
  private:
    
    bool damageShip_  = false;
    bool damageEnemy_ = false;
    
    std::shared_ptr<glm::vec3>  shipPosition_;
    glm::vec3                   targetPosition_;
    unsigned int                spawnerID_;
    
  public:
    Projectile( PhysicsObjectParams physicsObjectParams, glm::vec3 targetPosition, bool generateSpread, unsigned int spawnerID );
    ~Projectile(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    void  calculateRotation( GLfloat dt );
    
    CollisionProperties collisionProperties();
    
};

#endif // PROJECTILE_HPP