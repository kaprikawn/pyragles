#ifndef GLTF_HPP
#define GLTF_HPP

#include <string>
#include <fstream>
#include <vector>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <memory>
#include "../deps/json.hpp"

class Gltf {
  private:
    
    // https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#binary-gltf-layout
    
    // json
    unsigned int        jsonStartByte_          = 12;
    unsigned int        jsonChunkDataStartByte_ = 20;
    unsigned int        jsonChunkLength_;
    unsigned int        jsonChunkType_;
    
    // binary data
    unsigned int        binStartByte_;
    unsigned int        binChunkDataStartByte_;
    unsigned int        binChunkLength_;
    unsigned int        binChunkType_;
    
    unsigned int        vertexDataSize_ = 0;
    
    std::string               name_;
    int                       mesh_;
    std::vector<glm::vec3>    positions_;
    std::vector<glm::vec3>    normals_;
    std::vector<glm::vec2>    texcoord_0s_;
    std::vector<unsigned int> indices_;
    //glm::vec3                 colour_;
    
    bool                      useUvData_ = false;
    std::vector<float>        vertexData_;
    
    std::ifstream           fs_;
    nlohmann::json          json_;
    
    unsigned char*          textureData_;
    
    int                     textureWidth_;
    int                     textureHeight_;
    int                     textureBpp_;
    
  public:
    
    Gltf();
    ~Gltf();
    
    bool init( const std::string& filename );
    
    std::vector<glm::vec3>  positions( unsigned int positionIndex, unsigned int &positionsCount );
    std::vector<glm::vec2>  texcoord_0s( unsigned int texcoord_0Index, unsigned int &uvCount );
    std::vector<GLuint>     indices( unsigned int indicesIndex );
    glm::vec3               colour( std::string nodeName );
    void                    loadTexture();
    
    std::vector<GLfloat> floats( unsigned int byteOffset, unsigned int byteLength );
    std::vector<GLushort> ushorts( unsigned int byteOffset, unsigned int byteLength );
    
    unsigned char*      textureData() { return textureData_; }
    
    const void*         vertexData() {
      if( useUvData_ ) {
        return ( const void* )&vertexData_[0];
      } else {
        return ( const void* )&positions_[ 0 ][ 0 ];
      }
    }
    const unsigned int* indexData()       { return ( const unsigned int* )&indices_[ 0 ]; }
    unsigned char*      textureCoord_0()  { return ( unsigned char* )&texcoord_0s_; }
    
    unsigned int        vertexDataSize()      { return vertexDataSize_; }
    unsigned int        indexCount()          { return indices_.size(); }
    unsigned int        textureCoord_0Count() { return sizeof( glm::vec2 ) * texcoord_0s_.size(); }
    
    int                 textureWidth() { return textureWidth_; }
    int                 textureHeight() { return textureHeight_; }
};

#endif //GLTF_HPP