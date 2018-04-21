#include "meshLoader.hpp"
#include <iostream>

void MeshLoader::addVertex( glm::vec3 position, glm::vec3 colour, int shapeType ) {
  
  shapeVertices_[ shapeType ].push_back( position );
  
  Vertex vertex = { position, colour };
  
  vertices_[ shapeType ].push_back( vertex );
  
  if( !loadMesh_[ shapeType ] )
    loadMesh_[ shapeType ] = true;
}

void MeshLoader::generateMeshes() {
  
  glm::vec3 position;
  glm::vec3 colour;
  int currentShape;
  
  // ship //
  currentShape = SHIP;
  glm::vec3 vertexPositions[ MAX_SHAPE ][ 6 ];
  vertexPositions[ currentShape ][ 0 ] = { 0, 0, -1 };
  vertexPositions[ currentShape ][ 1 ] = { 0, 0.6f, 0.6f };
  vertexPositions[ currentShape ][ 2 ] = { 1, 0, 0.6f };
  vertexPositions[ currentShape ][ 3 ] = { -1, 0, 0.6f };
  vertexPositions[ currentShape ][ 4 ] = { 0, -0.4f, 0.6f };
  vertexPositions[ currentShape ][ 5 ] = { 0, 0, 1 };
  // front
  position  = { 0, 0, -1 };
  colour    = { 1.0f,  1.0f,  1.0f };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 0 ] = position;
  // 1 top
  position  = { 0, 0.6f, 0.6f };
  colour    = { 0, 0.4f, 0 };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 1 ] = position;
  // 2 right
  position  = { 1, 0, 0.6f };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 2 ] = position;
  // 3 left
  position  = { -1, 0, 0.6f };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 3 ] = position;
  // 4 bottom
  position  = { 0, -0.4f, 0.6f };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 4 ] = position;
  // 5 back
  position  = { 0, 0, 1 };
  colour    = { 1, 0, 0 };
  addVertex( position, colour, currentShape );
  vertexPositions[ currentShape ][ 5 ] = position;
  
  std::vector<GLuint> indices = {
      0, 1, 2 // f t r
    , 0, 1, 3 // f t l
    , 0, 4, 2 // f b r
    , 0, 4, 3 // f b l
    , 5, 1, 2 // b t r
    , 5, 1, 3 // b t l
    , 5, 4, 2 // b b r
    , 5, 4, 3 // b b l
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    std::cout << indices[ i ] << ", " << indices[ i + 1 ] << ", " << indices[ i + 2 ] << std::endl;
    
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions[ currentShape ][ indices[ i ] ]
        , vertexPositions[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  
  indices.clear();
  
  std::cout << mesh_[ currentShape ][ 0 ][ 1 ].z << std::endl;
  
  // target //
  currentShape  = TARGET;
  colour        = { 0.729f, 0.129f, 0.176f };
  
  position = { -0.9f,  0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f,  0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.8f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.8f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f, -0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f, -0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.8f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.8f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0,  1,  2
    , 2,  1,  3
    , 1,  4,  5
    , 4,  5,  6
    , 5,  8,  7
    , 7,  8,  9
    , 10, 11, 8
    , 10, 8,  3
  };
  indices_[ currentShape ] = indices;
  indices.clear();
  
  // bullet //
  currentShape  = BULLET;
  
  GLfloat scaler = 0.4f;
  
  position  = {  0.0f,  0.0f, -1.0f };
  colour    = { 1, 0, 0 };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  colour    = { 0.0f, 0.0f, 1.0f };
  
  position  = {  0.0f,  0.6f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  1.0f,  0.0f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = { -1.0f,  0.0f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  0.0f, -0.4f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  0.0f,  0.0f,  1.0f };
  colour    = {  1.0f,  1.0f,  1.0f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2 // f t r
    , 0, 1, 3 // f t l
    , 0, 4, 2 // f b r
    , 0, 4, 3 // f b l
    , 5, 1, 2 // b t r
    , 5, 1, 3 // b t l
    , 5, 4, 2 // b b r
    , 5, 4, 3 // b b l
  };
  indices_[ currentShape ] = indices;
  indices.clear();
  
  // arch
  currentShape  = ARCH;
  colour        = { 1, 0, 0 };
  
  position  = { -3, 0, 1 };
  addVertex( position, colour, currentShape );
  position  = { -2, 0, 1 };
  addVertex( position, colour, currentShape );
  position  = { -3, 3, 1 };
  addVertex( position, colour, currentShape );
  position  = { -2, 2.7, 1 };
  addVertex( position, colour, currentShape );
  position  = { -2, 4, 1 };
  addVertex( position, colour, currentShape );
  position  = { -1.6, 3.1, 1 };
  addVertex( position, colour, currentShape );
  position  = { 2, 4, 1 };
  addVertex( position, colour, currentShape );
  position  = { 1.6, 3.1, 1 };
  addVertex( position, colour, currentShape );
  position  = { 3, 2.7, 1 };
  addVertex( position, colour, currentShape );
  position  = { 2, 2.7, 1 };
  addVertex( position, colour, currentShape );
  position  = { 3, 0, 1 };
  addVertex( position, colour, currentShape );
  position  = { 2, 0, 1 };
  addVertex( position, colour, currentShape );

  colour = { 0, 0, 1 };
  
  position  = { -3.4, 0, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { -3, 0, 1 };
  addVertex( position, colour, currentShape );
  position  = { -3.4, 3.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { -3, 3, 1 };
  addVertex( position, colour, currentShape );
  position  = { -2.4, 4.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { -2, 4, 1 };
  addVertex( position, colour, currentShape );
  position  = { 2.4, 4.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 2, 4, 1 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 3.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3, 2.7, 1 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 0, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3, 0, 1 };
  addVertex( position, colour, currentShape );
  position  = { -3.4, 0, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { -3.4, 0, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { -3.4, 3.4, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { -3.4, 3.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { -2.4, 4.4, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { -2.4, 4.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 2.4, 4.4, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { 2.4, 4.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 3.4, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 3.4, 0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 0, -0.3 };
  addVertex( position, colour, currentShape );
  position  = { 3.4, 0, 0.3 };
  addVertex( position, colour, currentShape );

  indices = {
      0, 1, 2
    , 1, 3, 2
    , 2, 3, 4
    , 3, 5, 4
    , 4, 5, 6
    , 5, 7, 6
    , 6, 7, 8
    , 7, 9, 8
    , 8, 9, 10
    , 9, 11, 10
    
    , 12, 13, 14
    , 13, 15, 14
    , 14, 15, 16
    , 15, 17, 16
    , 16, 17, 18
    , 17, 19, 18
    , 18, 19, 20
    , 19, 21, 20
    , 20, 21, 22
    , 21, 23, 22
  };
  indices_[ currentShape ] = indices;
  
  // floor 1
  currentShape = FLOOR1;
  colour = { 0.87f, 0.733f, 0.129f };
  
  position = { -1.0f,  0.0f, -1.0f };
  addVertex( position, colour, currentShape );
  position = { -1.0f,  0.0f,  1.0f };
  addVertex( position, colour, currentShape );
  position = {  1.0f,  0.0f,  1.0f };
  addVertex( position, colour, currentShape );
  position = {  1.0f,  0.0f, -1.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2
    , 3, 2, 0
  };
  indices_[ currentShape ] = indices;
  indices.clear();
  
  // floor 2
  currentShape = FLOOR2;
  colour = { 0.87f, 0.623f, 0.129f };
  
  position = { -100.0f,  0.0f, -100.0f };
  addVertex( position, colour, currentShape );
  position = { -100.0f,  0.0f, 10.0f };
  addVertex( position, colour, currentShape );
  position = {  100.0f,  0.0f, 10.0f };
  addVertex( position, colour, currentShape );
  position = {  100.0f,  0.0f, -100.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2
    , 3, 2, 0
  };
  indices_[ currentShape ] = indices;
  indices.clear();
  
  setOffsets();
}

void MeshLoader::setOffsets() {
  GLintptr vertexOffset = 0;
  GLintptr indexOffset  = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      vertexOffsets_[ i ] = vertexOffset;
      indexOffsets_[ i ]  = indexOffset;
      
      bufferData_[ i ].vertexOffset = vertexOffset;
      bufferData_[ i ].indexOffset  = indexOffset;
      bufferData_[ i ].numIndices   = indices_[ i ].size();
      
      vertexOffset  += vertices_[ i ].size() * sizeof( vertices_[ i ][ 0 ] );
      indexOffset   += indices_[ i ].size() * sizeof( indices_[ i ][ 0 ] );
    }
  }
}

GLsizeiptr MeshLoader::totalVertexBufferSize() {
  GLsizeiptr vertexBufferSize = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      vertexBufferSize += vertices_[ i ].size() * sizeof( vertices_[ i ][ 0 ] );
    }
  }
  
  return vertexBufferSize;
}

GLsizeiptr MeshLoader::totalIndexBufferSize() {
  GLsizeiptr indexBufferSize = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      indexBufferSize += indices_[ i ].size() * sizeof( indices_[ i ][ 0 ] );
    }
  }
  
  return indexBufferSize;
}
