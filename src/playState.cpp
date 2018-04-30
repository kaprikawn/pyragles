#include "playState.hpp"
#include <iostream>
#include "shader.hpp"
#include "global.hpp"
#include "projectile.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera ) {
  
  std::shared_ptr<Shader> shader = std::make_shared<Shader>();
  
  viewProjectionMatrix_ = camera -> viewProjectionMatrix();
  
  GLuint programID = shader -> init();
  
  if( programID == 0 )
    return false;
  
  meshLoader_ = std::make_shared<MeshLoader>();
  meshLoader_ -> generateMeshes();
  
  renderer_ = std::make_shared<Renderer>( programID, camera );
  renderer_   -> generateBuffer( meshLoader_ -> totalVertexBufferSize(), meshLoader_ -> totalIndexBufferSize() );
  renderer_   -> addBufferData( meshLoader_ );
  
  Collision collision;
  collision_ = collision;
  
  shipPosition_           = std::make_shared<glm::vec3>();
  GLfloat shipStartZ      = -3.0f;
  GLfloat targetDistance  = 15.0f;
  
  int shapeType = TARGET;
  PhysicsObjectParams params;
  params.shapeType    = shapeType;
  params.initPosition = { -3, 0, shipStartZ - targetDistance };
  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.inputHandler = inputHandler;
  params.shipPosition = shipPosition_;
  
  target_ = std::make_shared<Target>( params );
  addPhysicsObject( target_, true, true );
  params = {};
  
  shapeType = SHIP;
  params.shapeType    = shapeType;
  params.initPosition = { -3, 0, shipStartZ };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.inputHandler = inputHandler;
  params.shipPosition = shipPosition_;
  
  ship_ = std::make_shared<Ship>( params, target_ );
  addPhysicsObject( ship_, true, true );
  params = {};
  
  shapeType = ARCH;
  params.shapeType    = shapeType;
  params.initPosition = { -3, FLOOR_Y, -40 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  params.inputHandler = inputHandler;
  params.shipPosition = shipPosition_;
  
  addPhysicsObject( std::make_shared<Scenary>( params ), true, true );
  params = {};
  
  shapeType = FLOOR1;
  params.shapeType    = shapeType;
  params.initPosition = { 0, FLOOR_Y, -10 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  
  addPhysicsObject( std::make_shared<Floor>( params, params.shapeType ), true, true );
  params = {};
  
  shapeType = FLOOR2;
  params.shapeType    = shapeType;
  params.initPosition = { 0, FLOOR_Y - 0.02, -10 };
  mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
  params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
  params.mesh         = mesh;
  params.renderer     = renderer_;
  addPhysicsObject( std::make_shared<Floor>( params, params.shapeType ), true, true );
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  // delete stuff that needs deleting
  for( unsigned i = liveObjects_.size(); i-- > 0; ) {
    if( liveObjects_[i] -> deleteObject() ) {
      liveObjects_[i]   -> clean();
      liveObjects_.erase( liveObjects_.begin() + i );
    }
  }
  
  if( liveObjects_[ 1 ] -> fire() ) { // fire bullets
    
    PhysicsObjectParams params;
    params.shapeType    = BULLET;
    params.initPosition = { shipPosition_ -> x, shipPosition_ -> y, shipPosition_ -> z };
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>( params.initPosition, meshLoader_ -> vertices( params.shapeType ), meshLoader_ -> mesh( params.shapeType ) );
    params.bufferData   = meshLoader_ -> bufferData( params.shapeType );
    params.mesh         = mesh;
    params.renderer     = renderer_;
    params.shipPosition = shipPosition_;
    
    std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>( params, target_ );
    PlayState::addPhysicsObject( newProjectile, true, false );
  }
  
  ship_ -> calculateRotation( dt );
  
  // collisions - starting at 1 because target collision is irrelevent
  for( unsigned int i = 1; i < liveObjects_.size(); i++ ) {
    for( unsigned int j = i + 1; j < liveObjects_.size(); j++ ) {
      
      if( liveObjects_[ i ] -> shapeType() == SHIP && liveObjects_[ j ] -> shapeType() == BULLET )
        continue;
      
      CollisionData collisionData = collision_.collisionData( liveObjects_[ i ], liveObjects_[ j ] );
      
      if( collisionData.isColliding() ) {
        
        CollisionProperties collisionProperties = liveObjects_[ i ] -> collisionProperties();
        liveObjects_[ j ] -> registerCollision( collisionData, collisionProperties );
        
        collisionProperties = liveObjects_[ j ] -> collisionProperties();
        liveObjects_[ i ] -> registerCollision( collisionData, collisionProperties );
      }
    }
  }
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ ) {
    liveObjects_[ i ] -> update( dt );
  }
}

void PlayState::render() {
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ )
    liveObjects_[ i ] -> render( viewProjectionMatrix_ );
}

void PlayState::addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading ) {
  
  physicsObject -> setObjectID( nextObjectID_++ );
  
  if( isLoading )
    levelObjects_.push_back( physicsObject );
  
  if( init )
    liveObjects_.push_back( physicsObject );
}

bool PlayState::onExit() {
  
  return true;
}