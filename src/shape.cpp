#include "shape.hpp"
#include <iostream>

Shape::Shape( int shapeType ) {
  
  if( shapeType == TARGET ) {
    
    glm::vec3 colour = { 0.8f, 0.8f, 0.8f };
    Vertex vertex;
    
    // 6 top left outer
    vertex.position    = { -0.9f,  0.9f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 7 top left lower
    vertex.position    = { -0.9f,  0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 8 top right upper
    vertex.position    = {  0.9f,  0.9f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 9 top right lower
    vertex.position    = {  0.9f,  0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 10 top left inner
    vertex.position    = { -0.8f,  0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 11 bottom left upper
    vertex.position    = { -0.9f, -0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 12 bottom left inner
    vertex.position    = { -0.8f, -0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 13 bottom left lower
    vertex.position    = { -0.9f, -0.9f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 14 bottom right upper
    vertex.position    = {  0.9f, -0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 15 bottom right lower
    vertex.position    = {  0.9f, -0.9f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 16 top right inner
    vertex.position    = {  0.8f,  0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 17 bottom right inner
    vertex.position    = {  0.8f, -0.8f, 0.0f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    indices_ = {
        0,  1,  2 // 0
      , 2,  1,  3 // 1
      , 1,  4,  5 // 2
      , 4,  5,  6 // 3
      , 5,  8,  7 // 4
      , 7,  8,  9 // 5
      , 10, 11, 8
      , 10, 8,  3
      
    };
    
  } else if( shapeType == SHIP ) {
    
    Vertex vertex;
    
    // 0 front
    vertex.position  = {  0.0f,  0.0f, -1.0f };
    vertex.colour    = {  1.0f,  1.0f,  1.0f };
    vertices_.push_back( vertex );
    
    // 1 top
    vertex.position    = {  0.0f,  0.6f,  0.6f };
    vertex.colour      = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( vertex );
    
    // 2 right
    vertex.position  = {  1.0f,  0.0f,  0.6f };
    vertex.colour    = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( vertex );
    
    // 3 left
    vertex.position   = { -1.0f,  0.0f,  0.6f };
    vertex.colour     = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( vertex );
    
    // 4 bottom
    vertex.position = {  0.0f, -0.4f,  0.6f };
    vertex.colour   = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( vertex );
    
    // 5 back
    vertex.position   = {  0.0f,  0.0f,  1.0f };
    vertex.colour     = {  1.0f,  0.0f,  0.0f };
    vertices_.push_back( vertex );
    
    indices_ = {
        0,  1,  2 // f t r
      , 0,  1,  3 // f t l
      , 0,  4,  2 // f b r
      , 0,  4,  3 // f b l
      , 5,  1,  2 // b t r
      , 5,  1,  3 // b t l
      , 5,  4,  2 // b b r
      , 5,  4,  3 // b b l
    };
    
  } else if( shapeType == CUBE ) {
    
    // front vertices
    Vertex topLeftF; // 0
    topLeftF.position     = { -1.0f,  1.0f,  1.0f };
    topLeftF.colour       = {  1.0f,  0.0f,  0.0f };
    vertices_.push_back( topLeftF );
    
    Vertex bottomLeftF; // 1
    bottomLeftF.position  = { -1.0f, -1.0f,  1.0f };
    bottomLeftF.colour    = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( bottomLeftF );
    
    Vertex topRightF; // 2
    topRightF.position    = {  1.0f,  1.0f,  1.0f };
    topRightF.colour      = {  0.0f,  0.0f,  1.0f };
    vertices_.push_back( topRightF );
    
    Vertex bottomRightF; // 3
    bottomRightF.position = {  1.0f, -1.0f,  1.0f };
    bottomRightF.colour   = {  1.0f,  1.0f,  1.0f };
    vertices_.push_back( bottomRightF );
    
    // back vertices
    Vertex topRightB; // 4
    topRightB.position    = {  1.0f,  1.0f, -1.0f };
    topRightB.colour      = {  1.0f,  0.0f,  0.0f };
    vertices_.push_back( topRightB );
    
    Vertex bottomRightB; // 5
    bottomRightB.position = {  1.0f, -1.0f, -1.0f };
    bottomRightB.colour   = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( bottomRightB );
    
    Vertex topLeftB; // 6
    topLeftB.position     = { -1.0f,  1.0f, -1.0f };
    topLeftB.colour       = {  1.0f,  1.0f,  0.0f };
    vertices_.push_back( topLeftB );
    
    Vertex bottomLeftB; // 7
    bottomLeftB.position  = { -1.0f, -1.0f, -1.0f };
    bottomLeftB.colour    = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( bottomLeftB );
    
    indices_ = {
        0, 1, 2 // front A
      , 2, 1, 3 // front B
      , 2, 3, 4 // right A
      , 4, 3, 5 // right B
      , 4, 5, 6 // back A
      , 6, 5, 7 // back B
      , 6, 7, 0 // left A
      , 0, 7, 1 // left B
      , 5, 3, 7 // bottom A
      , 7, 3, 1 // bottom B
      , 2, 4, 0 // top A
      , 0, 4, 6 // top B
    };
    
  } else if( shapeType == TRIANGLE ) {
    
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
    
    indices_ = { 0, 1, 2 };
    
  }
  
  numVertices_  = vertices_.size();
  numIndices_   = indices_.size();
}
