//
// Created by mikhail on 6/21/20.
//

#include <string>

#include "config_manager.h"
#include "json_reader.h"
#include "draw_functions.h"


int main(int n_args, char** args){
  if( n_args < 2 ){
    return 1;
  }
  std::string input{args[1]};
  ConfigManager::Instance()->Draw(input);
  return 0;
}