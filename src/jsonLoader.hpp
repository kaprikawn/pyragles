#ifndef JSONLOADER_HPP
#define JSONLOADER_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <vector>
#include "physicsObject.hpp"

class JsonLoader {
  
  private:
    
    std::vector<PhysicsObjectParams> params_;
    
    std::vector<glm::vec3> shapeVertices_[ MAX_SHAPE ];
    
    std::vector<Vertex> vertices_[ MAX_SHAPE ];
    std::vector<GLuint> indices_[ MAX_SHAPE ];
    
    GLintptr  vertexOffsets_[ MAX_SHAPE ];
    GLintptr  indexOffsets_[ MAX_SHAPE ];
    
    BufferData bufferData_[ MAX_SHAPE ];
    
    std::vector<std::array<glm::vec3, 3>> mesh_[ MAX_SHAPE ];
    std::vector<glm::vec3> vertexPositions_[ MAX_SHAPE ];
    
    bool loadMesh_[ MAX_SHAPE ] = { false };
    
  public:
    
    void addVertex( glm::vec3 position, glm::vec3 colour, int shapeType );
    
    void loadLevel( int levelNumber );
    
};

#endif //JSONLOADER_HPP