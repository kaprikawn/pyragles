#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include <GLES2/gl2.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "shader.hpp"
#include "mesh.hpp"

class Renderer;

enum ShapeTypes {
    UNDEF_SHAPE, SHIP, TARGET, BULLET
  , ARCH
  , FLOOR1, FLOOR2
  , MAX_SHAPE_NUM
  , MAX_SHAPE = MAX_SHAPE_NUM
};

enum ObjectState {
  UNDEF_STATE, NORMAL, COLLIDED
};

struct Vertex {
  glm::vec3   position;
  glm::vec3   colour;
};

struct BufferData {
  GLsizeiptr vertexOffset;
  GLsizeiptr indexOffset;
  GLsizei    numIndices;
};

class PhysicsObject {
  protected:
    
    unsigned int  objectID_;
    unsigned int  objectState_    = UNDEF_STATE;
    unsigned int  newObjectState_ = UNDEF_STATE;
    
    bool          delete_         = false;
    bool          fire_           = false;
    int           shapeType_      = 0;
    BufferData    bufferData_;
    
    glm::vec3     acceleration_   = { 0.0f, 0.0f, 0.0f };
    glm::vec3     velocity_       = { 0.0f, 0.0f, 0.0f };
    
    std::shared_ptr<Mesh>     mesh_;
    std::shared_ptr<Renderer> renderer_;
    
    glm::mat4                 modelMatrix_;
    glm::mat4                 rotationMatrix_;
    
  public:
    PhysicsObject( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> );
    virtual ~PhysicsObject(){}
    
    virtual void  update( GLfloat dt, bool skipMove = false );
    virtual void  render( glm::mat4 viewProjectionMatrix );
    virtual void  clean();
    virtual void  calculateRotation( GLfloat dt );
    virtual void  registerCollision( CollisionData collisionData );
    
    
    bool fire() {
      if( fire_ ) {
        fire_ = false;
        return true;
      }
      return false;
    }
    
    AABB aabb() { return mesh_ -> aabb(); }
    
    bool deleteObject() { return delete_; }
    
};

#endif // PHYSICSOBJECT_HPP