#ifndef GLTF_HPP
#define GLTF_HPP

#include <string>
#include <fstream>
#include <vector>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include "../deps/json.hpp"

struct GltfNode {
  std::string             name;
  int                     mesh;
  int                     positionIndex;
  int                     normalIndex;
  int                     texcoord_0Index;
  int                     indicesIndex;
  std::vector<glm::vec3>  positions;
  std::vector<glm::vec3>  normals;
  std::vector<glm::vec2>  texcoord_0s;
  std::vector<GLuint>     indices;
  glm::vec3               colour;
};

class Gltf {
  private:
    
    // https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#binary-gltf-layout
    
    // header
    uint32_t  magic_;
    uint32_t  version_;
    uint32_t  length_;
    
    // json
    uint32_t        jsonStartByte_          = 12;
    uint32_t        jsonChunkDataStartByte_ = 20;
    uint32_t        jsonChunkLength_;
    uint32_t        jsonChunkType_;
    
    // binary data
    uint32_t        binStartByte_;
    uint32_t        binChunkDataStartByte_;
    uint32_t        binChunkLength_;
    uint32_t        binChunkType_;
    
    std::ifstream   fs_;
    nlohmann::json  json_;
    
    std::vector<GltfNode> gltfNodes_;
    
  public:
    
    Gltf( const std::string& filename );
    ~Gltf(){}
    
    GltfNode                gltfNode( int mesh, std::string name );
    std::vector<glm::vec3>  positions( int positionIndex );
    std::vector<GLuint>     indices( int indicesIndex, GLuint starting_index );
    
    void dataDumpBinary();
    std::vector<GLfloat> floats( uint32_t byteOffset, uint32_t byteLength );
    std::vector<GLushort> ushorts( uint32_t byteOffset, uint32_t byteLength );
    
    std::vector<GltfNode> gltfNodes() { return gltfNodes_; }
};

#endif //GLTF_HPP