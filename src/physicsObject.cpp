#include "physicsObject.hpp"
#include <iostream>
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "renderer.hpp"
#include "camera.hpp"

PhysicsObject::PhysicsObject( PhysicsObjectParams physicsObjectParams, bool print ) {
  
  bufferData_   = physicsObjectParams.bufferData;
  renderer_     = physicsObjectParams.renderer;
  mesh_         = physicsObjectParams.mesh;
  renderer_     = physicsObjectParams.renderer;
  shapeType_    = physicsObjectParams.shapeType;
  canFire_      = physicsObjectParams.canFire;
  
  modelMatrix_  = glm::translate( glm::mat4(), mesh_ -> position() );
  
  mesh_ -> updatePosition( glm::vec3( 0, 0, 0 ), 0.0f, true );
}

void PhysicsObject::update( GLfloat dt, bool skipMove ) {
  mesh_ -> updatePosition( velocity_, dt, skipMove );
  mesh_ -> updateVertices( modelMatrix_, true );
  
  if( objectState_ == DEAD )
    delete_ = true;
}

void PhysicsObject::render( glm::mat4 viewProjectionMatrix ) {
  
  modelMatrix_ = glm::translate( glm::mat4(), mesh_ -> position() );
  modelMatrix_ *= rotationMatrix_;
  
  glm::mat4 mvp = viewProjectionMatrix * modelMatrix_;
  
  renderer_ -> renderObject( bufferData_.vertexOffset, bufferData_.indexOffset, bufferData_.numIndices, mvp );
  
  newObjectState_ = UNDEF_STATE;
}

void PhysicsObject::registerCollision( CollisionData collisionData, CollisionProperties collisionProperties ) {
  
}

void PhysicsObject::calculateRotation( GLfloat dt ) {
  
}

CollisionProperties PhysicsObject::collisionProperties() {
  CollisionProperties collisionProperties;
  return collisionProperties;
}

void PhysicsObject::clean() {
  
}
