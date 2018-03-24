#include "shape.hpp"
#include <iostream>

Shape::Shape( int shapeType ) {
  
  if( shapeType == SHIP ) {
    
    // 0 front
    Vertex front;
    front.position  = {  0.0f,  0.0f, -1.0f };
    front.colour    = {  1.0f,  1.0f,  1.0f };
    vertices_.push_back( front );
    
    // 1 top
    Vertex top;
    top.position    = {  0.0f,  0.6f,  0.6f };
    top.colour      = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( top );
    
    // 2 right
    Vertex right;
    right.position  = {  1.0f,  0.0f,  0.6f };
    right.colour    = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( right );
    
    // 3 left
    Vertex left;
    left.position   = { -1.0f,  0.0f,  0.6f };
    left.colour     = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( left );
    
    // 4 bottom
    Vertex bottom;
    bottom.position = {  0.0f, -0.4f,  0.6f };
    bottom.colour   = {  0.0f,  1.0f,  0.0f };
    vertices_.push_back( bottom );
    
    // 5 back
    Vertex back;
    back.position   = {  0.0f,  0.0f,  1.0f };
    back.colour     = {  1.0f,  0.0f,  0.0f };
    vertices_.push_back( back );
    
    // f t r
    // f t l
    
    
    indices_ = {
        0, 1, 2 // f t r
      , 0, 1, 3 // f t l
      , 0, 4, 2 // f b r
      , 0, 4, 3 // f b l
      , 5, 1, 2 // b t r
      , 5, 1, 3 // b t l
      , 5, 4, 2 // b b r
      , 5, 4, 3 // b b l
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
