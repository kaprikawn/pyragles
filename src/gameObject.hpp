#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "glCallLog.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "camera.hpp"

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
    
    float xAngle_ = 0.0f;
    float yAngle_ = 0.0f;
    float zAngle_ = 0.0f;
    
    glm::mat4 modelMatrix_;
    glm::mat4 rotationMatrix_;
    glm::mat4 mvp_;
    glm::vec4 acceleration_;
    glm::vec4 velocity_;
    glm::vec4 position_;
    
    unsigned int  indexCount_ = 0;
    
    std::vector<glm::vec4>  originalCollider_; // at start before any transformations
    std::vector<glm::vec4>  collider_; // current collider (add position and rotation)
    bool hasCollider_ = false;
    
    bool          debugCollider_ = true;
    VertexBuffer  vbCol_;
    Shader        shaderCol_;
    int           positionIDCol_;
    int           mvpIDCol_;
    
  public:
    
    GameObject();
    virtual ~GameObject();
    
    bool  loadModelFromGltf( std::string modelName );
    void  updatePosition( glm::vec4 velocity, float dt, bool skip = false );
    virtual void update( float dt );
    virtual void render( glm::mat4 viewProjectionMatrix );
    
    void loadVertexData( const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW );
    void loadIndexData( const void* data, unsigned int count );
    void loadShader( const std::string& filename );
    void loadTexture( unsigned char* textureData, int width, int height );
    
    unsigned int indexCount() const { return indexCount_; }
    
    void updateCollider( int debug = 1 );
    std::vector<glm::vec4> collider() { return collider_; }
    bool hasCollider() { return hasCollider_; }
    
};

#endif //GAMEOBJECT_HPP