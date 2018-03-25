#ifndef POSITION_HPP
#define POSITION_HPP

#include <GLES2/gl2.h>
#include "vector.hpp"

class Position {
  
  private:
    
    Vector  coordinates_;
  
  public:
    Position( GLfloat initX, GLfloat initY, GLfloat initZ );
    ~Position(){}
    
    void updatePosition( Vector &velocity, float &dt );
    
    Vector  coordinateValues() { return coordinates_; }
  
};

#endif //POSITION_HPP
