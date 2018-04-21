#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <GLES2/gl2.h>
#include <memory>
#include "physicsObject.hpp"
#include "renderer.hpp"

class Floor : public PhysicsObject {
  private:
    
  public:
    
    Floor( PhysicsObjectParams physicsObjectParams, int shapeType );
    ~Floor(){}
    
    void  update( GLfloat dt, bool skipMove = false );
    void  render( glm::mat4 viewProjectionMatrix );
    void  clean();
    
};

#endif // FLOOR_HPP