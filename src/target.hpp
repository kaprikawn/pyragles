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
    Target( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer, std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<glm::vec3> shipPosition );
    virtual ~Target(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    
    void handleInput( GLfloat dt );
    
    glm::vec3 position() {
      return mesh_ -> position();
    }
};

#endif // TARGET_HPP