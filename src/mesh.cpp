#include "mesh.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh( glm::vec3 initPosition, std::vector<glm::vec3> vertices, bool print ) {
  
  position_     = initPosition;
  vertices_     = vertices;
  originalVertices_ = vertices_;
  
}

void Mesh::updatePosition(  glm::vec3 velocity, GLfloat dt, bool skip ) {
  if( !skip )
    position_ += velocity * dt;
}

void Mesh::updateMesh( glm::mat4 modelMatrix ) {
  
  AABB aabb;
  bool firstPass = true;
  
  for( unsigned int i = 0; i < vertices_.size(); i++ ) {
    
    vertices_[ i ] = glm::vec3( modelMatrix * glm::vec4( originalVertices_[i], 1 ) );
    
    if( firstPass || vertices_[ i ].x > aabb.right ) {
      aabb.right = vertices_[ i ].x;
    }
    
    if( firstPass || vertices_[ i ].x < aabb.left ) {
      aabb.left = vertices_[ i ].x;
    }
    
    if( firstPass || vertices_[ i ].y > aabb.top ) {
      aabb.top = vertices_[ i ].y;
    }
    
    if( firstPass || vertices_[ i ].y < aabb.bottom ) {
      aabb.bottom = vertices_[ i ].y;
    }
      
    if( firstPass || vertices_[ i ].z > aabb.back ) {
      aabb.back = vertices_[ i ].z;
    }
      
    if( firstPass || vertices_[ i ].z < aabb.front ) {
      aabb.front = vertices_[ i ].z;
    }
    
    firstPass = false;
  }
  
  aabb_ = aabb;
}

void Mesh::setPosition( glm::vec3 coordinates ) {
  position_ = coordinates;
}

void Mesh::setX( GLfloat x ) {
  position_.x = x;
}

void Mesh::setY( GLfloat y ) {
  position_.y = y;
}