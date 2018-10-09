#include "physicsObject.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hpp"
#include "camera.hpp"

PhysicsObject::PhysicsObject( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer ) {
  
  mesh_       = std::make_shared<Mesh>( initPosition, mesh );
  bufferData_ = bufferData;
  renderer_   = renderer;
  
}

void PhysicsObject::update( GLfloat dt, bool skipMove ) {
  mesh_ -> updatePosition( velocity_, dt, skipMove );
  mesh_ -> updateMesh( modelMatrix_ );
}

void PhysicsObject::render( glm::mat4 viewProjectionMatrix ) {
  
  modelMatrix_ = glm::translate( glm::mat4( 1.0f ), mesh_ -> position() );
  modelMatrix_ *= rotationMatrix_;
  
  glm::mat4 mvp = viewProjectionMatrix * modelMatrix_;
  
  renderer_ -> renderObject( bufferData_.vertexOffset, bufferData_.indexOffset, bufferData_.numIndices, mvp );
}

void PhysicsObject::clean() {
  
}

void PhysicsObject::calculateRotation( GLfloat dt ) {
  
}