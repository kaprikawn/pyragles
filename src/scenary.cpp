#include "scenary.hpp"
#include <iostream>

Scenary::Scenary( int shapeType, GLuint programID, glm::vec3 startPos ) : GlObject( shapeType, startPos.x, startPos.y, startPos.z, programID ) {
  velocity_.setZ( SCROLL_SPEED );
}

void Scenary::updatePosition( float dt ) {
  position_.updatePosition( velocity_, dt );
}

void Scenary::update( float dt ) {
  Scenary::updatePosition( dt );
  model_ = glm::translate( glm::mat4(), position_.coordinates() );
  GlObject::update( dt );
}

void Scenary::render() {
  GlObject::render();
}

void Scenary::clean() {
  GlObject::clean();
}