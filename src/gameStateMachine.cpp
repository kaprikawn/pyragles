#include <iostream>
#include "gameStateMachine.hpp"

void GameStateMachine::pushState( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler ) {
  gameStates_.push_back( std::move( state ) );
  gameStates_.back() -> onEnter( inputHandler );
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

void GameStateMachine::changeState( std::unique_ptr<GameState> state, std::shared_ptr<InputHandler> inputHandler ) {
  std::cout << "Changing game state to " << state -> getStateID() << std::endl;
  if( !gameStates_.empty() ) {
    if( gameStates_.back() -> getStateID() == state -> getStateID() ) {
      return;
    }
    if( gameStates_.back() -> onExit() ) {
      gameStates_.pop_back();
    }
  }
  
  gameStates_.push_back( std::move( state ) );
  
  gameStates_.back() -> onEnter( inputHandler );
  
  std::cout << "finished changing state - stateID is now " << gameStates_.back() -> getStateID() << std::endl;
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
