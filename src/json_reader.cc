//
// Created by mikhail on 8/21/20.
//

#include "json_reader.h"
#include "marker_constants.h"
#include <boost/property_tree/json_parser.hpp>

namespace JsonConfig {

Draw::Picture GetPictureConfig(const std::string &json_file) {
  boost::property_tree::ptree config;
  boost::property_tree::read_json(json_file, config);
  Draw::Picture picture;
  // save name
  picture.save_name = config.get<std::string>("save name", "canv.png");
  // resolution
  try {
    auto resolution_configs = config.get_child("resolution");
    int idx = 0;
    for (const auto &res_conf : resolution_configs) {
      picture.resolution.at(idx) = res_conf.second.get<short>("");
      idx++;
    }
  } catch (const std::exception&) {
    picture.resolution = {1000, 1100};
  }
  // text
  try {
    auto text_config = config.get_child("top text");
    picture.text = text_config.get<std::string>("text", "");
    picture.text_position = {text_config.get<double>("x1", 0.0),
                             text_config.get<double>("y1", 0.0)};
  } catch (const std::exception&) {}
  // legend_position
  try {
    auto legend_configs = config.get_child("legend position");
    for( const auto& leg_conf : legend_configs ){
      picture.legend_position.emplace_back( leg_conf.second.get<double>("") );
    }
  } catch (const std::exception&) {}
  // y axis range
  try{
    auto axis_range_config = config.get_child("y axis range");
    for( const auto& axis_conf : axis_range_config ){
      picture.y_axis_range.emplace_back( axis_conf.second.get<double>("") );
    }
  } catch (const std::exception&) {}
  // x axis range
  try{
    auto axis_range_config = config.get_child("x axis range");
    for( const auto& axis_conf : axis_range_config ){
      picture.x_axis_range.emplace_back( axis_conf.second.get<double>("") );
    }
  } catch (const std::exception&) {}
  // axis names
  picture.axis_titles = config.get<std::string>("axis title", "");
  return picture;
};

Draw::Histogram2D GetHistogram2DConfig( const std::string& json_file ){
  boost::property_tree::ptree config;
  boost::property_tree::read_json(json_file, config);
  Draw::Histogram2D histogram;

  auto histo_config = config.get_child( "histogram 2D" );
  histogram.file = histo_config.get<std::string>("file");
  histogram.name = histo_config.get<std::string>("name");
  return histogram;
};

std::vector<Draw::Correlation>
GetCorrelationConfigs( const std::string& json_file ){
  boost::property_tree::ptree config;
  boost::property_tree::read_json(json_file, config);
  std::vector<Draw::Correlation> correlations;
  auto corr_config = config.get_child("correlations");
  for( const auto& conf : corr_config ){
    auto correlation_config = conf.second;
    correlations.emplace_back();
    correlations.back().name = correlation_config.get<std::string>("name");
    correlations.back().title = correlation_config.get<std::string>("title");
    correlations.back().file = correlation_config.get<std::string>("file");
    auto rebin_config = correlation_config.get_child("rebin axis");
    for( const auto& axis : rebin_config ){
      auto title = axis.second.get<std::string>("name");
      auto n_bins = axis.second.get<int>("n bins");
      auto low_bin = axis.second.get<double>("low bin");
      auto up_bin = axis.second.get<double>("up bin");
      correlations.back().rebin_axes.emplace_back(title, n_bins, low_bin, up_bin);
    }
    correlations.back().projection_axis = correlation_config.get<std::string>("projection axis", "");
    correlations.back().scale = correlation_config.get<double>("scale", 1.0);
    correlations.back().marker = MarkerConstants::MARKERS.at(correlation_config.get<std::string>("marker"));
    correlations.back().color = MarkerConstants::COLORS.at(correlation_config.get<std::string>("color"));
  }
  return correlations;
};

Draw::Style GetSyleConfig( const std::string& json_file ){
  boost::property_tree::ptree config;
  boost::property_tree::read_json(json_file, config);
  Draw::Style style_config;
  style_config.pad_left_margin = config.get<float>("pad left margin");
  style_config.pad_right_margin = config.get<float>("pad right margin");
  style_config.pad_bottom_margin = config.get<float>("pad bottom margin");
  style_config.legend_border_size = config.get<int>("legend border size");
  style_config.frame_line_width = config.get<int>("frame line width");
  style_config.marker_size = config.get<int>("marker size");
  style_config.line_width = config.get<int>("line width");
  auto title_size_config = config.get_child("title size");
  style_config.title_size.at(0) = title_size_config.get<float>("X");
  style_config.title_size.at(1) = title_size_config.get<float>("Y");

  auto title_offset_config = config.get_child("title offset");
  style_config.title_offset.at(0) = title_offset_config.get<float>("X");
  style_config.title_offset.at(1) = title_offset_config.get<float>("Y");

  return style_config;
}

}