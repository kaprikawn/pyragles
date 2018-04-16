#include "shape.hpp"
#include <iostream>

// http://doc.instantreality.org/tools/color_calculator/

Shape::Shape( int shapeType ) {
  
  shapeType_ = shapeType;
  
  Vertex vertex;
  
  if( shapeType == TARGET ) {
    
    glm::vec3 colour = { 0.729f, 0.129f, 0.176f };
    
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
    
    glm::vec3 middle = { 0.0f, 0.4f, 0.0f };

    // 1 top
    vertex.position    = {  0.0f,  0.6f,  0.6f };
    vertex.colour      = middle;
    vertices_.push_back( vertex );
    
    // 2 right
    vertex.position  = {  1.0f,  0.0f,  0.6f };
    vertex.colour    = middle;
    vertices_.push_back( vertex );
    
    // 3 left
    vertex.position   = { -1.0f,  0.0f,  0.6f };
    vertex.colour     = middle;
    vertices_.push_back( vertex );
    
    // 4 bottom
    vertex.position = {  0.0f, -0.4f,  0.6f };
    vertex.colour   = middle;
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
  
    glm::vec3 colour = { 0.031f, 0.070f, 0.749f };
    
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
    vertex.colour     = { 0.031f, 0.537f, 0.749f };
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
    
  } else if( shapeType == FLOOR1 ) {
    
    glm::vec3 colour = { 0.87f, 0.733f, 0.129f };

    vertex.position = { -1.0f,  0.0f, -1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = { -1.0f,  0.0f,  1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = {  1.0f,  0.0f,  1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = {  1.0f,  0.0f, -1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    indices_ = {
        0, 1, 2
      , 0, 2, 3
    };
    
  } else if( shapeType == FLOOR2 ) {
    
    glm::vec3 colour = { 0.87f, 0.623f, 0.129f };

    vertex.position = { -100.0f,  0.0f, -100.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = { -100.0f,  0.0f, 10.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = {  100.0f,  0.0f, 10.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    vertex.position = {  100.0f,  0.0f, -100.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    indices_ = {
        0, 1, 2
      , 0, 2, 3
    };
  
  } else if( shapeType == ARCH ) {
    
    glm::vec3 colour = { 1.0f, 0.0f, 0.0f };
    
    // front face
    
    // 0
    vertex.position = { -3.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 1
    vertex.position = { -2.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 2
    vertex.position = { -3.0f, 3.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 3
    vertex.position = { -2.0f, 2.7f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 4
    vertex.position = { -2.0f, 4.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 5
    vertex.position = { -1.6f, 3.1f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 6
    vertex.position = { 2.0f, 4.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 7
    vertex.position = { 1.6f, 3.1f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 8
    vertex.position = { 3.0f, 2.7f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 9
    vertex.position = { 2.0f, 2.7f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 10
    vertex.position = { 3.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 11
    vertex.position = { 2.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );
    
    // second back
    colour = { 0.0f, 1.0f, 0.0f };
    
    // 12
    vertex.position = { -3.4f, 0.0f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 13
    vertex.position = { -3.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 14
    vertex.position = { -3.4f, 3.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 15
    vertex.position = { -3.0f, 3.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 16
    vertex.position = { -2.4f, 4.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 17
    vertex.position = { -2.0f, 4.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 18
    vertex.position = { 2.4f, 4.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 19
    vertex.position = { 2.0f, 4.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 20
    vertex.position = { 3.4f, 3.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 21
    vertex.position = { 3.0f, 2.7f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 22
    vertex.position = { 3.4f, 0.0f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 23
    vertex.position = { 3.0f, 0.0f, 1.0f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );
    
    // top
    
    colour = { 0.0f, 0.0f, 1.0f };
    
    // 24
    vertex.position = { -3.4f, 0.0f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 25
    vertex.position = { -3.4f, 0.0f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 26
    vertex.position = { -3.4f, 3.4f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 27
    vertex.position = { -3.4f, 3.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 28
    vertex.position = { -2.4f, 4.4f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 29
    vertex.position = { -2.4f, 4.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 30
    vertex.position = { 2.4f, 4.4f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 31
    vertex.position = { 2.4f, 4.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 32
    vertex.position = { 3.4f, 3.4f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 33
    vertex.position = { 3.4f, 3.4f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 34
    vertex.position = { 3.4f, 0.0f, -0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );

    // 35
    vertex.position = { 3.4f, 0.0f, 0.3f };
    vertex.colour   = colour;
    vertices_.push_back( vertex );
    
    indices_ = {
         0,  1,  2
      ,  2,  1,  3
      ,  4,  2,  3
      ,  4,  3,  5
      ,  4,  5,  6
      ,  6,  5,  7
      ,  6,  7,  8
      ,  8,  7,  9
      ,  8,  9, 10
      ,  9, 11, 10
      
      , 12, 13, 14
      , 14, 13, 15
      , 14, 15, 16
      , 15, 17, 16
      , 16, 17, 18
      , 17, 19, 18
      , 18, 19, 20
      , 19, 21, 20
      , 20, 21, 22
      , 21, 23, 22
      
      , 24, 25, 26
      , 25, 27, 26
      , 26, 27, 28
      , 27, 28, 29
      , 28, 29, 30
      , 29, 31, 30
      , 30, 31, 32
      , 31, 33, 32
      , 32, 33, 34
      , 33, 35, 34
      
      
    };
  }
  
  numVertices_  = vertices_.size();
  numIndices_   = indices_.size();
}