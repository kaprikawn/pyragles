#include "mesh.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh( glm::vec3 initPosition, std::vector<glm::vec3> mesh, bool print ) {
  
  position_     = initPosition;
  mesh_         = mesh;
  originalMesh_ = mesh_;
  
  //glm::mat4 modelMatrix_ =  glm::translate( glm::mat4(), position_ );
  //updateMesh( glm::translate( glm::mat4(), position_ ) );
  
}

void Mesh::updatePosition(  glm::vec3 velocity, GLfloat dt, bool skip ) {
  if( !skip )
    position_ += velocity * dt;
}

void Mesh::updateMesh( glm::mat4 modelMatrix ) {
  
  AABB aabb;
  bool firstPass = true;
  
  for( unsigned int i = 0; i < mesh_.size(); i++ ) {
    
    mesh_[ i ] = glm::vec3( modelMatrix * glm::vec4( originalMesh_[i], 1 ) );
    
    if( firstPass || mesh_[ i ].x > aabb.right ) {
      aabb.right = mesh_[ i ].x;
    }
    
    if( firstPass || mesh_[ i ].x < aabb.left ) {
      aabb.left = mesh_[ i ].x;
    }
    
    if( firstPass || mesh_[ i ].y > aabb.top ) {
      aabb.top = mesh_[ i ].y;
    }
    
    if( firstPass || mesh_[ i ].y < aabb.bottom ) {
      aabb.bottom = mesh_[ i ].y;
    }
      
    if( firstPass || mesh_[ i ].z > aabb.back ) {
      aabb.back = mesh_[ i ].z;
    }
      
    if( firstPass || mesh_[ i ].z < aabb.front ) {
      aabb.front = mesh_[ i ].z;
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