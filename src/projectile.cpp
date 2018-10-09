#include <iostream>
#include "projectile.hpp"

Projectile::Projectile( int shapeType, GLuint programID, glm::vec3 origin, glm::vec3 destination ) : GlObject( shapeType, origin.x, origin.y, origin.z, programID ) {
  
  velocity_.setX( destination.x - origin.x );
  velocity_.setY( destination.y - origin.y );
  velocity_.setZ( destination.z - origin.z );

  velocity_.normalize();
  
  velocity_*= 15.0f;
  
}

void Projectile::updatePosition( float dt ) {
  position_.updatePosition( velocity_, dt );
}

void Projectile::update( float dt ) {
  
  Projectile::updatePosition( dt );
  
  glm::mat4 scale_ = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.2f ) ); 
  
  model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
  model_ *= rotation_;
  model_ *= scale_;
  
  if( position_.coordinates().z < -40.0f )
    delete_ = true;
  
  GlObject::update( dt );
  
}

void Projectile::render() {
  GlObject::render();
}

void Projectile::clean() {
  GlObject::clean();
}
