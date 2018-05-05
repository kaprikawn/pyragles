#include "floor.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Floor::Floor( PhysicsObjectParams physicsObjectParams, int shapeType ) : PhysicsObject( physicsObjectParams ) {
  
  if( shapeType == FLOOR1 )
    velocity_.z = SCROLL_SPEED;
}

void Floor::update( GLfloat dt, bool skipMove ) {
  PhysicsObject::update( dt, skipMove );
}

void Floor::render( glm::mat4 viewProjectionMatrix ) {
  
  if( shapeType_ == FLOOR2 ) {
    modelMatrix_ = glm::translate( glm::mat4(), mesh_ -> position() );
    PhysicsObject::render( viewProjectionMatrix );
    return;
  }
  
  GLfloat initX = mesh_ -> x();
  GLfloat initY = mesh_ -> y();
  GLfloat initZ = mesh_ -> z();
  
  if( initZ > -5.0f ) {
    initZ -= 4.0f;
  }
  
  GLfloat x = initX;
  GLfloat y = initY;
  GLfloat z = initZ;

  bool odd = false;

  for( int i = 0; i < 35; i++ ) {
    for( int n = -22; n < 22; n++ ) {
      mesh_ -> setPosition( glm::vec3( x + n * 4.0f, y, z ) );
      modelMatrix_ = glm::translate( glm::mat4(), mesh_ -> position() );
      PhysicsObject::render( viewProjectionMatrix );
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
  
  mesh_ -> setPosition( glm::vec3( initX, initY, initZ ) );
}

void Floor::clean() {
  PhysicsObject::clean();
}