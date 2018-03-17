#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "structs.hpp"
#include <GLES2/gl2.h>
#include <vector>

class Shape {
  private:
  
    std::vector<Vertex>  vertices_;
    std::vector<GLuint>  indices_;
    
    GLuint  numVertices_  = 0;
    GLuint  numIndices_   = 0;
    
  public:
    Shape( int shapeType );
    ~Shape(){}
    
  GLsizei getStride() {
    return sizeof( Vertex );
  }
  
  GLfloat* getVertexDataPointer() {
    return &vertices_[0].position.x; // returns pointer to the first position
  }
  
  GLuint* getIndexDataPointer() {
    return &indices_[0];
  }
  
  GLsizeiptr vertexBufferSize() {
    return vertices_.size() * sizeof( vertices_[0] );
  }
  
  GLsizeiptr indexBufferSize() {
    return indices_.size() * sizeof( indices_[0] );
  }
  
  GLuint numIndices() {
    return numIndices_;
  }
  
  GLvoid* colorOffset() {
    return ( char* )( sizeof( GLfloat ) * 3 );
  }
};

#endif //SHAPE_HPP
