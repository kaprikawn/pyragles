#ifndef PHYSICSCOMPONENT_HPP
#define PHYSICSCOMPONENT_HPP

#include "../component.hpp"
#include <glm/glm.hpp>

class PhysicsComponent : public Component {
  
  private :
  
    glm::vec4 velocity_ = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    
  public :
    PhysicsComponent();
    ~PhysicsComponent();
    
    
    void update( float dt );
    
    void setVelocity( float x, float y, float z ) {
      velocity_ = glm::vec4( x, y, z, 1.0f );
    }
    
    
};
    
#endif //PHYSICSCOMPONENT_HPP
