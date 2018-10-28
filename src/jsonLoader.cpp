#include "jsonLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../deps/json.hpp"

void JsonLoader::loadLevel( int levelNumber ) {
  std::cout << "loading level " << levelNumber << std::endl;
  
  std::stringstream ss;
  ss << "assets/level" << levelNumber << ".json";
  std::string filename = ss.str();
  
  std::ifstream fin( filename, std::ifstream::binary );
  nlohmann::json j;
  fin >> j;
}