#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP


#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "camera.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class GameObject {
  protected:
  
    VertexBuffer  vb_;
    IndexBuffer   ib_;
    Shader        shader_;
    Texture       texture_;
    std::unique_ptr<Model> model_;
    
    int  positionID_;
    int  normalID_;
    int  texCoordID_;
    int  colourID_;
    int  mvpID_;
    
    float yAngle_ = 0;
    
    glm::mat4     modelMatrix_;
    glm::mat4     rotationMatrix_;
    glm::mat4     mvp_;
    
    unsigned int  indexCount_ = 0;
    
  public:
    
    GameObject();
    virtual ~GameObject();
    
    bool loadModelFromGltf( std::string modelName );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    void loadVertexData( const void* data, unsigned int size );
    void loadIndexData( const void* data, unsigned int count );
    void loadShader( const std::string& filename );
    void loadTexture( unsigned char* textureData, int width, int height );
    
    unsigned int indexCount() const { return indexCount_; }
    
};

#endif //GAMEOBJECT_HPP