#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

class Movement {
  private:
  
    glm::vec3   startVelocity_    = { 0, 0, 0 };
    glm::vec3   targetVelocity_   = { 0, 0, 0 };
    glm::vec3   changeMagnitude_  = { 0, 0, 0 };
    
  public:
  
    Movement(){}
    ~Movement(){}
    
    void defineChange( glm::vec3 currentVelocity, glm::vec3 targetVelocity, glm::vec3 changeMagnitude );
    glm::vec3 changeVelocity( glm::vec3 currentVelocity, GLfloat dt );
};

#endif // MOVEMENT