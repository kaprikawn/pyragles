#ifndef GLTF_HPP
#define GLTF_HPP

#include <string>
#include <fstream>
#include <GLES2/gl2.h>
#include "../deps/json.hpp"

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
    
  public:
    
    Gltf( const std::string& filename );
    ~Gltf(){}
    
    void dataDumpBinary();
    std::vector<GLfloat> floats( uint32_t byteOffset, uint32_t byteLength );
};

#endif //GLTF_HPP