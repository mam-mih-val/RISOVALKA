//
// Created by mikhail on 8/23/20.
//

#include <string>

#include "json_reader.h"
#include "draw_functions.h"

int main(int n_args, char** args){
  if( n_args < 2 ){
    return 1;
  }
  std::string input{args[1]};
  auto picture_config = JsonConfig::GetPictureConfig(input);
  auto correlation_configs = JsonConfig::GetCorrelationConfigs(input);
  auto style = JsonConfig::GetSyleConfig("/home/mikhail/flow_drawing_tools/src/config/style.json");
  Draw::SetStyle(style);
  Draw::Draw1D(picture_config, correlation_configs, {}, {});
  return 0;
}
