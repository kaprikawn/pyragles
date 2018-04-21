#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include <GLES2/gl2.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "shader.hpp"
#include "mesh.hpp"
#include "collisionData.hpp"
#include "inputHandler.hpp"

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

struct PhysicsObjectParams {
  glm::vec3   initPosition;
  BufferData  bufferData;
  std::shared_ptr<Mesh>         mesh;
  std::shared_ptr<Renderer>     renderer;
  std::shared_ptr<InputHandler> inputHandler;
  std::shared_ptr<glm::vec3>    shipPosition;
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
    
    unsigned int  lastCollisionID_;
    
    glm::mat4     modelMatrix_;
    glm::mat4     rotationMatrix_;
    
    
    glm::vec3     acceleration_   = { 0.0f, 0.0f, 0.0f };
    glm::vec3     velocity_       = { 0.0f, 0.0f, 0.0f };
    
    std::shared_ptr<Mesh>     mesh_;
    std::shared_ptr<Renderer> renderer_;
    
  public:
    PhysicsObject( PhysicsObjectParams physicsObjectParams, bool print = false );
    virtual ~PhysicsObject(){}
    
    virtual void  update( GLfloat dt, bool skipMove = false );
    virtual void  render( glm::mat4 viewProjectionMatrix );
    virtual void  clean();
    virtual void  calculateRotation( GLfloat dt );
    virtual void  registerCollision( CollisionData collisionData, CollisionProperties collisionProperties );
    
    virtual CollisionProperties collisionProperties();
    
    bool fire() {
      if( fire_ ) {
        fire_ = false;
        return true;
      }
      return false;
    }
    
    AABB aabb()             { return mesh_ -> aabb(); }
    bool deleteObject()     { return delete_; }
    unsigned int objectID() { return objectID_; }
    
    std::vector<glm::vec3> vertices() {
      return mesh_ -> vertices();
    }
    
    void setObjectID( unsigned int objectID ) {
      objectID_ = objectID;
    }
    
    void changeState( ObjectState state ) {
      objectState_    = state;
      newObjectState_ = state;
    }
    
};

#endif // PHYSICSOBJECT_HPP