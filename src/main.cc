//
// Created by mikhail on 8/23/20.
//

#include <string>

#include <boost/program_options.hpp>

#include "json_reader.h"
#include "draw_functions.h"


int main(int n_args, char** args){
  namespace po = boost::program_options;
  std::string draw_object;
  std::string input_config;
  po::options_description options("Options");
  options.add_options()
      ("help,h", "Help screen")
      ("type,t", po::value<std::string>(&draw_object),
          "Set type of drawable object: 1D for bunch of 1D objects or 2D for 2D histogram")
      ("input,i", po::value<std::string>(&input_config),
          "Set path to input configuration");
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(n_args, args).options(options).run();
  po::store(parsed, vm);
  po::notify(vm);
  if (vm.count("help")){
    std::cout << options << std::endl;
    return 0;
  }
  // 1-dimensional objects
  if( draw_object == "1D" or draw_object == "1d" or draw_object.empty() ) {
    auto style = JsonConfig::GetStyleConfig("../src/config/style_1d.json");
    Draw::SetStyle(style);
    auto picture_config = JsonConfig::GetPictureConfig(input_config);
    auto correlation_configs =
        JsonConfig::GetCorrelationConfigs(input_config, "correlations");
    std::vector<Draw::CorrelationConfig> ref_correlation_configs;
    try {
      ref_correlation_configs =
          JsonConfig::GetCorrelationConfigs(input_config, "reference");
    } catch (const std::exception&) {}
    if (std::empty(ref_correlation_configs))
      Draw::Draw1D(picture_config, correlation_configs, {}, {});
    else
      Draw::CompareCorrelations(picture_config, ref_correlation_configs,
                                correlation_configs);
    return 0;
  }
  // 2-dimensional objects
  if( draw_object == "2D" or draw_object == "2d" ) {
    auto style = JsonConfig::GetStyleConfig("../src/config/style_2d.json");
    Draw::SetStyle(style);
    auto picture_config = JsonConfig::GetPictureConfig(input_config);
    auto histogram_config = JsonConfig::GetHistogram2DConfig(input_config);
    Draw::DrawHistogram2D(picture_config, histogram_config);
    return 0;
  }

}
