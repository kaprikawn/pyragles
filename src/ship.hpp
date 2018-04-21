#ifndef SHIP_HPP
#define SHIP_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"
#include "inputHandler.hpp"
#include "target.hpp"

class Ship : public PhysicsObject {
  private:
    
    std::shared_ptr<InputHandler> inputHandler_;
    std::shared_ptr<glm::vec3>    shipPosition_;
    std::shared_ptr<Target>       target_;
    
  public:
    Ship( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer, std::shared_ptr<InputHandler> inputHander, std::shared_ptr<glm::vec3>    shipPosition, std::shared_ptr<Target> target );
    ~Ship(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    void  calculateRotation( GLfloat dt );
    
    void handleInput( GLfloat dt );
};

#endif // SHIP_HPP