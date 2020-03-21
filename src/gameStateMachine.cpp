#include <iostream>
#include "gameStateMachine.hpp"

bool GameStateMachine::pushState( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler ) {
  gameStates_.push_back( std::move( state ) );
  bool loadSuccessful = gameStates_.back() -> onEnter( inputHandler, nextLevel_ );
  if( !loadSuccessful )
    return false;
  
  return true;
}

void GameStateMachine::popState() {
  if( !gameStates_.empty() ) {
    if( gameStates_.back() -> onExit() ) {
      gameStates_.pop_back();
    }
  }
}

std::string GameStateMachine::getCurrentGameStateID() {
  if( !gameStates_.empty() ) {
    return gameStates_.back() -> getStateID();
  }
  return NULL;
}

bool GameStateMachine::changeState( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler ) {
  std::cout << "Changing game state to " << state -> getStateID() << std::endl;
  if( !gameStates_.empty() ) {
    if( gameStates_.back() -> getStateID() == state -> getStateID() )
      return true;
    if( gameStates_.back() -> onExit() )
      gameStates_.pop_back();
  }
  
  gameStates_.push_back( std::move( state ) );
  
  bool changeSuccessful = gameStates_.back() -> onEnter( inputHandler, nextLevel_ );
  if( !changeSuccessful )
    return false;
  
  if( gameStates_.back() -> getStateID() == "PLAY" ) {
    nextLevel_ = gameStates_.back() -> nextLevel();
  }
  
  std::cout << "finished changing state - stateID is now " << gameStates_.back() -> getStateID() << std::endl;
  return true;
}

void GameStateMachine::update( float dt ) {
  if( !gameStates_.empty() ) {
    gameStates_.back() -> update( dt );
  }
}

void GameStateMachine::render() {
  if( !gameStates_.empty() ) {
    gameStates_.back() -> render();
  }
}
