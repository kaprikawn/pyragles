#include "playState.hpp"
#include <iostream>
#include "shader.hpp"
#include "camera.hpp"
#include "global.hpp"
#include "collisionData.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler ) {
  
  std::shared_ptr<Camera> camera = std::make_shared<Camera>();
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
  
  collision_ = std::make_unique<Collision>();
  
  shipPosition_           = std::make_shared<glm::vec3>();
  GLfloat shipStartZ      = -3.0f;
  GLfloat targetDistance  = 15.0f;
  
  int shapeType = TARGET;
  target_ = std::make_shared<Target>( glm::vec3( -3, 0, shipStartZ - targetDistance ), meshLoader_ -> bufferData( shapeType ), meshLoader_ -> mesh( shapeType ), renderer_, inputHandler, shipPosition_ );
  addPhysicsObject( target_, true, true );
  
  shapeType = SHIP;
  ship_ = std::make_shared<Ship>( glm::vec3( -3, 0, shipStartZ ), meshLoader_ -> bufferData( shapeType ), meshLoader_ -> mesh( shapeType ), renderer_, inputHandler, shipPosition_, target_ );
  addPhysicsObject( ship_, true, true );
  
  shapeType = ARCH;
  addPhysicsObject( std::make_shared<Scenary>( glm::vec3( -3, -5, -40 ), meshLoader_ -> bufferData( shapeType ), meshLoader_ -> mesh( shapeType ), renderer_ ), true, true );
  
  shapeType = FLOOR1;
  addPhysicsObject( std::make_shared<Floor>( glm::vec3( 0, FLOOR_Y, -10 ), meshLoader_ -> bufferData( shapeType ), meshLoader_ -> mesh( shapeType ), renderer_, shapeType ), true, true );
  
  shapeType = FLOOR2;
  addPhysicsObject( std::make_shared<Floor>( glm::vec3( 0, FLOOR_Y - 0.02, 0 ), meshLoader_ -> bufferData( shapeType ), meshLoader_ -> mesh( shapeType ), renderer_, shapeType ), true, true );
  
  
  
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ ) {
    liveObjects_[ i ] -> update( dt );
  }
  
  ship_ -> calculateRotation( dt );
  
  /* CollisionData collisionData = collision_ -> collisionData( ship_, arch_ );
  
  if( collisionData.isColliding() ) {
    ship_ -> registerCollision( collisionData );
    //arch_ -> registerCollision( collisionData );
  } */
}

void PlayState::render() {
  
  for( unsigned int i = 0; i < liveObjects_.size(); i++ )
    liveObjects_[ i ] -> render( viewProjectionMatrix_ );
    
}

void PlayState::addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading ) {
  
  if( isLoading )
    levelObjects_.push_back( physicsObject );
  
  if( init )
    liveObjects_.push_back( physicsObject );
  
}

bool PlayState::onExit() {
  
  return true;
}