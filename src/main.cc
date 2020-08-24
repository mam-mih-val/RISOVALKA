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
  auto correlation_configs = JsonConfig::GetCorrelationConfigs(input, "correlations");
  auto ref_correlation_configs = JsonConfig::GetCorrelationConfigs(input, "reference");
  auto style = JsonConfig::GetStyleConfig("../src/config/style.json");
  Draw::SetStyle(style);
  if( std::empty(ref_correlation_configs) )
    Draw::Draw1D(picture_config, correlation_configs, {}, {});
  else
    Draw::CompareCorrelations(picture_config, ref_correlation_configs, correlation_configs);
  return 0;
}
