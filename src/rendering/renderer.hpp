#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include "geometry.hpp"
#include "renderable.hpp"

class Renderer {
  
  private :
    static const unsigned int NUM_MAX_GEOMETRIES = 10;
    Geometry geometries_[ NUM_MAX_GEOMETRIES ];
    unsigned int numGeometries_;
    static const unsigned int NUM_MAX_RENDERABLES = 10;
    Renderable renderables_[ NUM_MAX_RENDERABLES ];
    unsigned int numRenderables_;
    static const unsigned int MAX_BUFFER_SIZE = 1024;
    
    unsigned int vertexBufferID_;
    unsigned int indexBufferID_;
    
    static const unsigned int MAX_VERTS = 10;
    
  protected :
    
    void initializeGL();
    void paintEvent();
    void paintGL();
  
  public :
    Renderer();
    ~Renderer();
    
    bool initialize();
    bool shutdown();
    
    Geometry* addGeometry( glm::vec4* vertices, unsigned int numVerts, unsigned short* indices, unsigned int numIndices, unsigned int rendermode );
    Renderable* addRenderable( Geometry* geometry );
    
};
    
#endif //RENDERER_HPP
