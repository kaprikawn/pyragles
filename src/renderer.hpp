#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <memory>
#include "shader.hpp"
#include "camera.hpp"
#include "physicsObject.hpp"
#include "meshLoader.hpp"

enum BufferTypes {
  VERTEX, INDEX
};

class Renderer {
  private:
    
    GLuint      vbo_;
    GLuint      ibo_;
    
    GLuint      positionID_;
    GLuint      colourID_;
    GLuint      mvpID_;
    
    GLsizeiptr  vertexOffset_ = 0;
    GLsizeiptr  indexOffset_  = 0;
    
    std::shared_ptr<Camera> camera_;
    
  public:
    
    Renderer( GLuint programID, std::shared_ptr<Camera> camera );
    ~Renderer(){}
    
    void generateBuffer( GLsizeiptr vertexBufferSize, GLsizeiptr indexBufferSize, GLenum usage = GL_STATIC_DRAW );
    void addBufferData( std::shared_ptr<MeshLoader> meshLoader );
    void renderObject( GLsizeiptr vertexOffset, GLsizeiptr indexOffset, GLsizei numIndices, glm::mat4 mvp );
    
    
};

#endif //RENDERER_HPP