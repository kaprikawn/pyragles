#ifndef SCENARY_HPP
#define SCENARY_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"

class Scenary : public PhysicsObject {
  private:
    
  public:
    Scenary( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer );
    ~Scenary(){}
    
    void update( GLfloat dt, bool skipMove = false );
    void render( glm::mat4 viewProjectionMatrix );
    void clean();
    
    //void calculateRotation( GLfloat dt );
};

#endif // SCENARY_HPP