#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>
#include "gltf.hpp"
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"
#include "renderer.hpp"

class GameObject {
  
  private :
  
    glm::mat4                     modelMatrix_;
    glm::mat4                     viewProjectionMatrix_;
    glm::mat4                     mvp_;
    
    std::unique_ptr<Gltf>         gltf_;
    std::shared_ptr<VertexBuffer> vb_;
    std::shared_ptr<IndexBuffer>  ib_;
    std::shared_ptr<Shader>       shader_;
    
  public :
    GameObject();
    ~GameObject();
    
    void loadVertexData( const void* data, unsigned int size );
    void loadIndexData( const void* data, unsigned int count );
    void loadShader( const std::string& filename );
    
    void loadGltf( const std::string filename );
    void update( float dt );
    void render( glm::mat4 viewProjectionMatrix );
    
};
    
#endif //GAMEOBJECT_HPP
