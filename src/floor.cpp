#include "floor.hpp"
#include <iostream>

Floor::Floor( int shapeType, GLuint programID ) : GlObject( shapeType, 0.0f, -2.0f, -50.0f, programID ) {
  rotation_ = glm::mat4();

  velocity_.setZ( 10.0f );
}

void Floor::update( float dt ) {
  
  position_.updatePosition( velocity_, dt );

  model_ = glm::translate( glm::mat4(), position_.coordinates() );

  GlObject::update( dt );

}

void Floor::render() {
  GlObject::render();
}

void Floor::clean() {
  GlObject::clean();
}

