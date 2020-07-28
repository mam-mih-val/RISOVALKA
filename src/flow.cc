//
// Created by mikhail on 7/8/20.
//

#include <string>

#include "computation_manager.h"

int main(int n_args, char** args){
  if( n_args < 2 ){
    return 1;
  }
  std::string input{args[1]};
  ComputationManager manager;
  manager.Init(input);
  manager.Compute();
  return 0;
}
