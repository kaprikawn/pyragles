#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include "physicsObject.hpp"

class MeshLoader {
  
  private:
    
    std::vector<glm::vec3> shapeMeshes_[ MAX_SHAPE ];
    
    std::vector<Vertex> vertices_[ MAX_SHAPE ];
    std::vector<GLuint> indices_[ MAX_SHAPE ];
    
    GLintptr  vertexOffsets_[ MAX_SHAPE ];
    GLintptr  indexOffsets_[ MAX_SHAPE ];
    
    BufferData bufferData_[ MAX_SHAPE ];
    
    bool loadMesh_[ MAX_SHAPE ] = { false };
        
  public:
    
    MeshLoader(){}
    ~MeshLoader(){}
    
    void generateMeshes();
    void setOffsets();
    void addVertex( glm::vec3 position, glm::vec3 colour, int shapeType );
    
    GLsizeiptr totalVertexBufferSize();
    GLsizeiptr totalIndexBufferSize();
    
    GLintptr vertexOffset( int shapeType )  { return vertexOffsets_[ shapeType ]; }
    GLintptr indexOffset( int shapeType )   { return indexOffsets_[ shapeType ]; }
    
    bool loadShape( int shapeType ) { return loadMesh_[ shapeType ]; }
    
    GLsizeiptr vertexSize( int shapeType ) {
      return vertices_[ shapeType ].size() * sizeof( vertices_[ shapeType ][ 0 ] );
    }
    
    GLsizeiptr indexSize( int shapeType ) {
      return indices_[ shapeType ].size() * sizeof( indices_[ shapeType ][ 0 ] );
    }
    
    GLfloat* vertexData( int shapeType ) {
      return &vertices_[ shapeType ][ 0 ].position.x;
    }
    
    GLuint* indexData( int shapeType ) {
      return &indices_[ shapeType ][ 0 ];
    }
    
    BufferData bufferData( int shapeType ) {
      return bufferData_[ shapeType ];
    }
    
    std::vector<glm::vec3> mesh( int shapeType ) {
      return shapeMeshes_[ shapeType ];
    }
    
    
};

#endif // MESHLOADER