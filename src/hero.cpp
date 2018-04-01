#include <iostream>
#include <cmath>
#include "hero.hpp"
#include "inputHandler.hpp"

#define PI 3.141592653589793238462643383279

Hero::Hero( int shapeType, Target* target ) : GlObject( shapeType, 0.0f, 0.0f, -10.0f ) {
  target_ = target;
}

void Hero::calculateRotation() {
  
  yAngle_ = -( atan( ( target_ -> position().coordinates().x - position_.coordinates().x ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // face left and right
  
  xAngle_ = ( atan( ( target_ -> position().coordinates().y - position_.coordinates().y ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // pitch up and down
  
  rotation_ = glm::rotate( glm::mat4(), glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  //rotation_ = glm::rotate( rotation_, glm::radians( zAngle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Hero::update( float dt ) {
  
  glm::vec3 vector = target_ -> position().coordinates() - position_.coordinates();
  
  Hero::calculateRotation();
  
  model_ = glm::translate( glm::mat4(), position_.coordinates() );
  model_ *= rotation_;
  
  GlObject::update( dt );
  
}

void Hero::render() {
  GlObject::render();
}

void Hero::clean() {
  GlObject::clean();
}
