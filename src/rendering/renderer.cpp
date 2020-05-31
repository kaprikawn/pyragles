#include "renderer.hpp"
#include <iostream>
#include <cassert>
#include <GLES2/gl2.h>

Renderer::Renderer() {
  
}

bool Renderer::initialize() {
  
  // set to max so we know in 'shutdown' whether we've got a valid value in here
  vertexBufferID_ = 0xffffffff;
  indexBufferID_  = 0xffffffff;
  
  numGeometries_  = 0;
  numRenderables_ = 0;
  
  glClearColor( 0, 0, 0, 1 );
  
  return true;
}

void Renderer::initializeGL() {
  
  glGenBuffers( 1, &vertexBufferID_ );
  glGenBuffers( 1, &indexBufferID_ );
  
  glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID_ );
  
  glBufferData( GL_ARRAY_BUFFER, MAX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, MAX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW );
  
  glEnableVertexAttribArray( 0 ); // TODO : change to positionID from shader
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 ); // TODO : see above
  
}

void Renderer::paintEvent() {
  
}

void Renderer::paintGL() {
  
  glClear( GL_COLOR_BUFFER_BIT );
  
  glm::vec4 transformedVerts[ MAX_VERTS ];
  
  for( unsigned int i = 0; i < numRenderables_; i++ ) {
    
    const Renderable& r = renderables_[ i ];
    
    // indices
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, sizeof( unsigned short ) * r.what_ -> numIndices_, r.what_ -> indices_ );
    
    // vertices
    for( unsigned int j = 0; j < r.what_ -> numVerts_; j++ )
      transformedVerts[ j ]  = r.where_ * r.what_ -> vertices_[ j ];
    
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( glm::vec4 ) * r.what_ -> numVerts_, transformedVerts );
    
    glDrawElements( r.what_ -> rendermode_, r.what_ -> numIndices_, GL_UNSIGNED_SHORT, 0 );
  }
  
}

Geometry* Renderer::addGeometry( glm::vec4* vertices, unsigned int numVerts, unsigned short* indices, unsigned int numIndices, unsigned int rendermode ) {
  
  assert( numGeometries_ != NUM_MAX_GEOMETRIES );
  
  Geometry& g   = geometries_[ numGeometries_++ ];
  g.vertices_   = vertices;
  g.numVerts_   = numVerts;
  g.indices_    = indices;
  g.numIndices_ = numIndices;
  g.rendermode_ = rendermode;
  
  return &g;
}

Renderable* Renderer::addRenderable( Geometry* geometry ) {
  
  assert( numRenderables_ != NUM_MAX_RENDERABLES );
  
  Renderable& r = renderables_[ numRenderables_++ ];
  r.what_ = geometry;
  
  return &r;
}

bool Renderer::shutdown() {
  
  if( vertexBufferID_ != 0xffffffff )
    glDeleteBuffers( 1, &vertexBufferID_ );
  if( indexBufferID_ != 0xffffffff )
    glDeleteBuffers( 1, &indexBufferID_ );
  
  return true;
}

Renderer::~Renderer() {
  
}

