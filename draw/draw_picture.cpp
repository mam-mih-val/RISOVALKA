//
// Created by mikhail on 2/28/21.
//

#include <TROOT.h>
#include <stdexcept>
int main(int n_args, char** args){
  if( n_args < 2 )
    throw std::runtime_error( "No argumets provided" );
//  auto interpreter = gROOT->GetInterpreter();
  gROOT->Macro(args[1]);
  return 0;
}