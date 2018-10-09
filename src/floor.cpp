#include "floor.hpp"
#include <iostream>

Floor::Floor( int shapeType, GLuint programID, GLfloat y ) : GlObject( shapeType, 0.0f, y, -20.0f, programID ) {
  
  rotation_ = glm::mat4( 1.0f );
  
  shapeType_ = shapeType;

  if( shapeType == FLOOR1 )
    velocity_.setZ( 20.0f );

}

void Floor::update( float dt ) {
  
  if( shapeType_ == FLOOR1 )
    position_.updatePosition( velocity_, dt );

  GlObject::update( dt );
}

void Floor::render() {

  if( shapeType_ == FLOOR2 ) {
    model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
    GlObject::render();
    return;
  }

  GLfloat initX = position_.coordinates().x;
  GLfloat initY = position_.coordinates().y;
  GLfloat initZ = position_.coordinates().z;

  if( initZ > -5.0f ) {
    initZ -= 4.0f;
  }

  GLfloat x = initX;
  GLfloat y = initY;
  GLfloat z = initZ;

  bool odd = false;

  for( int i = -10; i < 15; i++ ) {
    for( int n = -13; n < 13; n++ ) {
      position_.setCoordinates( x + n * 4.0f, y, z );
      model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
      GlObject::render();
    }

    z -= 2.0f;
    x = initX;
    if( odd ) {
      odd = false;
    } else {
      x += 2.0f;
      odd = true;
    }
  }
  
  position_.setCoordinates( initX, initY, initZ );
}

void Floor::clean() {
  GlObject::clean();
}
