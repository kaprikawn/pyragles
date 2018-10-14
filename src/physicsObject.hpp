#ifndef PHYSICSOBJECT_HPP
#define PHYSICSOBJECT_HPP

#include <GLES2/gl2.h>
#include <SDL2/SDL.h>
#include "../deps/glm/glm.hpp"
#include <vector>
#include <memory>
#include "shader.hpp"
#include "mesh.hpp"
#include "collisionData.hpp"
#include "inputHandler.hpp"

class Renderer;

enum ShapeTypes {
    UNDEF_SHAPE, SHIP, TARGET, BULLET
  , ENEMY_POD
  , ARCH
  , FLOOR1, FLOOR2
  , MAX_SHAPE_NUM
  , MAX_SHAPE = MAX_SHAPE_NUM
};

enum ObjectState {
  UNDEF_STATE, NORMAL, COLLIDED, DYING, DEAD
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

enum ObjectType {
  UNDEF_OBJ_TYPE, FRIENDLY, ENEMY, SCENARY
};

struct PhysicsObjectParams {
  int         shapeType;
  int         objectType    = 0;
  bool        canFire       = false;
  bool        damageShip    = false;
  bool        damageEnemy   = false;
  GLfloat     velMultiplier = 0.0f;
  glm::vec3   initPosition;
  BufferData  bufferData;
  unsigned int                          spawnerID = 0;
  std::shared_ptr<Mesh>                 mesh;
  std::shared_ptr<Renderer>             renderer;
  std::shared_ptr<InputHandler>         inputHandler;
  std::shared_ptr<glm::vec3>            shipPosition;
};

class PhysicsObject {
  protected:
    
    unsigned int  objectID_;
    unsigned int  objectState_    = UNDEF_STATE;
    unsigned int  newObjectState_ = UNDEF_STATE;
    unsigned int  spawnerID_      = 0;
    
    bool          delete_         = false;
    bool          canFire_        = false;
    bool          fire_           = false;
    int           shapeType_      = 0;
    int           objectType_     = 0;
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
    int  shapeType()        { return shapeType_; }
    
    std::vector<std::array<glm::vec3, 3>> mesh() {
      return mesh_ -> mesh();
    }
    
    glm::vec3 position() {
      return mesh_ -> position();
    }
    
    bool  deleteObject()      { return delete_; }
    bool  canFire()           { return canFire_; }
    int   objectType()        { return objectType_; }
    unsigned int objectID()   { return objectID_; }
    unsigned int spawnerID()  { return spawnerID_; }
    
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