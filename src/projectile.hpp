#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "target.hpp"

class Projectile : public PhysicsObject {
  private:
    
    std::shared_ptr<Target> target_;
  public:
    
    Projectile(  glm::vec3 initPosition, int shapeType, std::shared_ptr<Renderer> renderer, std::shared_ptr<glm::vec3> shipPosition, std::shared_ptr<Target> target );
    virtual ~Projectile(){}
    
    virtual void  registerCollision();
    virtual void  update( GLfloat dt );
    virtual void  render();
    virtual void  clean();
    
    void calculateRotation( GLfloat dt );
};

#endif // PROJECTILE_HPP