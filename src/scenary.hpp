#ifndef SCENARY_HPP
#define SCENARY_HPP

#include "glObject.hpp"

class Scenary : public GlObject {
  
  private:
  
  
  public:
    Scenary( int shapeType, GLuint programID, glm::vec3 startPos );
    virtual ~Scenary(){}
    
    void updatePosition   ( float dt );
    
    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
  
};

#endif //SCENARY_HPP
