#include "renderer.hpp"
#include <iostream>
#include <cassert>
#include <GLES2/gl2.h>

Renderer::Renderer() {
  
}

bool Renderer::initialize() {
  
  vertexBufferID_ = -1;
  indexBufferID_  = -1;
  
  numGeometries_  = 0;
  numRenderables_ = 0;
  
  return true;
}

void Renderer::initializeGL() {
  
  glGenBuffers( 1, &vertexBufferID_ );
  glGenBuffers( 1, &indexBufferID_ );
  
  glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID_ );
  
  glBufferData( GL_ARRAY_BUFFER, MAX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, MAX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW );
  
  
}

void Renderer::paintEvent() {
  
}

void Renderer::paintGL() {
  
}

Geometry* Renderer::addGeometry( glm::vec4* vertices, unsigned int numVerts, unsigned short* indices, unsigned int numIndices ) {
  
  assert( numGeometries_ != NUM_MAX_GEOMETRIES );
  
  Geometry& g   = geometries_[ numGeometries_++ ];
  g.vertices_   = vertices;
  g.numVerts_   = numVerts;
  g.indices_    = indices;
  g.numIndices_ = numIndices;
  
  return &g;
}

Renderable* Renderer::addRenderable( Geometry* geometry ) {
  
  assert( numRenderables_ != NUM_MAX_RENDERABLES );
  
  Renderable& r = renderables_[ numRenderables_++ ];
  r.what_ = geometry;
  
  return &r;
}

bool Renderer::shutdown() {
  
  if( vertexBufferID_ != -1 )
    glDeleteBuffers( 1, &vertexBufferID_ );
  if( indexBufferID_ != -1 )
    glDeleteBuffers( 1, &indexBufferID_ );
  
  return true;
}

Renderer::~Renderer() {
  
}

