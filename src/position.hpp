#ifndef POSITION_HPP
#define POSITION_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include "vector.hpp"

class Position {
  
  private:
    
    Vector  coordinates_;
  
  public:
    Position( GLfloat initX, GLfloat initY, GLfloat initZ );
    ~Position(){}
    
    void updatePosition( Vector &velocity, float &dt );
    
    //Vector  coordinates() { return coordinates_; }
    
    glm::vec3 coordinates();
    
    void setCoordinates( GLfloat x, GLfloat y, GLfloat z ) {
      coordinates_.setX( x );
      coordinates_.setY( y );
      coordinates_.setZ( z );
    }
  
};

#endif //POSITION_HPP
