//
// Created by mikhail on 7/17/20.
//

#include "config_reader.h"

Correlation ConfigReader::ReadSingleCorrelation(const boost::property_tree::ptree& config){
  auto name = config.get<std::string>("name");
  auto title = config.get<std::string>("title");
  auto projection_axis =config.get<std::string>("projection axis", "");
  auto scale = config.get<double>("scale", 1.0);

  std::vector<Qn::Axis<double>> rebin_axis;
  try {
    auto rebin_config = config.get_child("rebin axis");
    for (const auto &axis : rebin_config) {
      auto axis_title = axis.second.get<std::string>("name");
      auto n_bins = axis.second.get<int>("n bins");
      auto low_bin = axis.second.get<double>("low bin");
      auto up_bin = axis.second.get<double>("up bin");
      rebin_axis.emplace_back(axis_title, n_bins, low_bin, up_bin);
    }
  } catch (const std::exception&) {
    std::cout << "ConfigReader::ReadSingleCorrelation: no rebin axis" << std::endl;
  }
  std::vector<std::string> components;
  try {
    auto components_config = config.get_child("components");
    for (const auto &component : components_config)
      components.emplace_back(component.second.get<std::string>(""));
  }catch (const std::exception&) {
    std::cout << "ConfigReader::ReadSingleCorrelation: no components found" << std::endl;
    abort();
  }
  return {title, name, components, rebin_axis, projection_axis, scale};
}
