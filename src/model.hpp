#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <fstream>
#include <vector>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <memory>
#include "../deps/json.hpp"

class Model {
  private:
    
    // https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md#binary-model-layout
    
    // json
    unsigned int        jsonStartByte_          = 12;
    unsigned int        jsonChunkDataStartByte_ = 20;
    unsigned int        jsonChunkLength_;
    unsigned int        jsonChunkType_;
    
    // binary data
    unsigned int        binStartByte_;
    unsigned int        binChunkDataStartByte_;
    
    unsigned int        vertexDataSize_ = 0;
    
    std::string               name_;
    int                       mesh_;
    std::vector<glm::vec4>    positions_;
    std::vector<glm::vec3>    normals_;
    std::vector<glm::vec2>    texcoord_0s_;
    std::vector<unsigned int> indices_;
    unsigned int              indexCount_ = 0;
    std::vector<glm::vec4>    collider_;
    bool                      hasCollider_ = false;
    unsigned int              colliderVertexCount_ = 0;
    
    bool                      useUvData_ = false;
    std::vector<float>        vertexData_;
    
    std::ifstream           fs_;
    nlohmann::json          json_;
    
    unsigned char*          textureData_;
    
    int                     textureWidth_;
    int                     textureHeight_;
    int                     textureBpp_;
    
  public:
    
    Model(){};
    ~Model();
    
    bool loadFromGltf( const std::string& filename );
    void loadModel( int mesh );
    void updateCollider();
    
    std::vector<glm::vec4>  positions( unsigned int positionIndex     , unsigned int &positionsCount );
    std::vector<glm::vec3>  normals( unsigned int normalIndex         , unsigned int &normalsCount );
    std::vector<glm::vec2>  texcoord_0s( unsigned int texcoord_0Index , unsigned int &uvCount );
    std::vector<GLuint>     indices( unsigned int indicesIndex );
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
    const void*     indexData()           { return ( const void* )&indices_[ 0 ]; }
    unsigned char*  textureCoord_0()      { return ( unsigned char* )&texcoord_0s_; }
    unsigned int    vertexDataSize()      { return vertexDataSize_; }
    unsigned int    indexCount()          { return indexCount_; }
    unsigned int    textureCoord_0Count() { return sizeof( glm::vec2 ) * texcoord_0s_.size(); }
    int             textureWidth()        { return textureWidth_; }
    int             textureHeight()       { return textureHeight_; }
    
    bool                    hasCollider() { return hasCollider_; }
    std::vector<glm::vec4>  collider()    { return collider_; }
    unsigned int            colliderVertexCount() { return colliderVertexCount_; }
};

#endif //MODEL_HPP