#include "shape.hpp"
#include <iostream>

Shape::Shape( int shapeType ) {
  
  shapeType_ = shapeType;
  
  Vertex vertex;
  
  if( shapeType == TARGET ) {
    
    glm::vec3 colour = { 0.8f, 0.8f, 0.8f };
    
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
    
  } else if( shapeType == BULLET ) {
  
    glm::vec3 colour = { 0.8f, 0.8f, 0.0f };
    
    // 0 front
    vertex.position  = {  0.0f,  0.0f, -1.0f };
    vertex.colour    = colour;
    vertices_.push_back( vertex );
    
    // 1 top
    vertex.position    = {  0.0f,  1.0f,  -0.75f };
    vertex.colour      = colour;
    vertices_.push_back( vertex );
    
    // 2 right
    vertex.position  = {  1.0f,  0.0f, -0.75f };
    vertex.colour    = colour;
    vertices_.push_back( vertex );
    
    // 3 left
    vertex.position   = { -1.0f,  0.0f, -0.75f };
    vertex.colour     = colour;
    vertices_.push_back( vertex );
    
    // 4 bottom
    vertex.position = {  0.0f, -1.0f, -0.75f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );
    
    // 5 back
    vertex.position   = {  0.0f,  0.0f,  1.0f };
    vertex.colour     = colour;
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
    
  }
  
  numVertices_  = vertices_.size();
  numIndices_   = indices_.size();
}
