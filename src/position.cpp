#include "position.hpp"
#include <iostream>

Position::Position( GLfloat x, GLfloat y, GLfloat z ) : coordinates_( x, y, z ) {

}

void Position::updatePosition( Vector &velocity, float &dt ) {
  coordinates_ = coordinates_ + velocity * dt;
}
