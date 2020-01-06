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
  
    std::unique_ptr<Gltf>         gltf_;
    std::shared_ptr<VertexBuffer> vb_;
    std::shared_ptr<IndexBuffer>  ib_;
    std::shared_ptr<Shader>       shader_;
    
  public :
    GameObject();
    ~GameObject();
    
    void loadGltf( const std::string filename );
    void draw( std::shared_ptr<Renderer> renderer );
    
};
    
#endif //GAMEOBJECT_HPP
