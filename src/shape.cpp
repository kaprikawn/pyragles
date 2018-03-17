#include "shape.hpp"
#include <iostream>


Shape::Shape( int shapeType ) {
  
  if( shapeType == TRIANGLE ) {
    
    Vertex top;
    top.position  = {  0.0f,  1.0f,  0.0f };
    top.colour    = {  1.0f,  0.0f,  0.0f };
    vertices_.push_back( top );
    
    Vertex bottomLeft;
    bottomLeft.position  = { -1.0f, -1.0f,  0.0f };
    bottomLeft.colour    = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( bottomLeft );
    
    Vertex bottomRight;
    bottomRight.position  = {  1.0f, -1.0f,  0.0f };
    bottomRight.colour    = {  0.0f,  0.0f,  1.0f };
    vertices_.push_back( bottomRight );
    
    numVertices_ = 3;
    
    indices_.push_back( 0 );
    indices_.push_back( 1 );
    indices_.push_back( 2 );
    
    numIndices_ = 3;
    
  }
}
