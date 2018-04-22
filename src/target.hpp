#ifndef TARGET_HPP
#define TARGET_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"

class Target : public PhysicsObject {
  private:
    
    std::shared_ptr<InputHandler> inputHandler_;
    
    std::shared_ptr<glm::vec3> shipPosition_;
    
  public:
    Target( PhysicsObjectParams physicsObjectParams );
    virtual ~Target(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    
    
    void handleInput( GLfloat dt );
    
    CollisionProperties collisionProperties();
    
    glm::vec3 position() {
      return mesh_ -> position();
    }
};

#endif // TARGET_HPP