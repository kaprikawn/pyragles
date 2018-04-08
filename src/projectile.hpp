#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "glObject.hpp"

class Projectile : public GlObject {
  
  private:
  
  
  public:
    Projectile( int shapeType, GLuint programID, glm::vec3 origin, glm::vec3 destination );
    virtual ~Projectile(){}
    
    void updatePosition   ( float dt );
    
    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
  
};

#endif //PROJECTILE_HPP
