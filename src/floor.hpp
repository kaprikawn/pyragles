#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "glObject.hpp"

class Floor : public GlObject {
  
  private:
    int shapeType_;
  
  public:
    
    Floor( int shapeType, GLuint programID, GLfloat y );
    virtual ~Floor(){}

    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
    
};

#endif //FLOOR_HPP