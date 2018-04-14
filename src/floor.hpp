#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "glObject.hpp"
#include "target.hpp"

class Floor : public GlObject {
  
  private:
    
  
  public:
    
    Floor( int shapeType, GLuint programID );
    virtual ~Floor(){}

    virtual void update   ( float dt );
    virtual void render();
    virtual void clean();
    
};

#endif //FLOOR_HPP
