//
// Created by mikhail on 8/21/20.
//

#include "json_reader.h"
#include "marker_constants.h"
#include <boost/property_tree/json_parser.hpp>

namespace JsonConfig {

Draw::Picture GetPictureConfig(const std::string &json_file) {
  boost::property_tree::ptree config;
  try {
    boost::property_tree::read_json(json_file, config);
  } catch( const std::exception& e  ){
    std::cout << "JsonConfig::GetPictureConfig()" << std::endl;
    throw e;
  }
  Draw::Picture picture;
  // save names
  picture.save_name = config.get<std::string>("save name", "canv.png");
  picture.save_points = config.get<bool>("save points", false);
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
    auto text_configs = config.get_child("labels");
    for(const auto& text_config : text_configs ) {
      picture.texts.emplace_back(text_config.second.get<std::string>("text", ""));
      std::vector text_position{text_config.second.get<double>("x1", 0.0),
                                text_config.second.get<double>("y1", 0.0)};
      picture.text_positions.emplace_back(text_position);
      picture.text_sizes.emplace_back( text_config.second.get<double>("size") );
    }
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
  // z axis range
  try{
    auto axis_range_config = config.get_child("z axis range");
    for( const auto& axis_conf : axis_range_config ){
      picture.z_axis_range.emplace_back( axis_conf.second.get<double>("") );
    }
  } catch (const std::exception&) {}
  // ratio y-axis range
  try{
    auto axis_range_config = config.get_child("ratio range");
    for( const auto& axis_conf : axis_range_config ){
      picture.ratio_range.emplace_back( axis_conf.second.get<double>("") );
    }
  } catch (const std::exception&) {}
  // axis names
  try {
    auto axes_titles = config.get_child("axes titles");
    for( const auto& axis_conf : axes_titles ){
      picture.axes_titles.emplace_back( axis_conf.second.get<std::string>("") );
    }
  }catch (const std::exception&) {}
  // tf1 formulas
  try {
    auto formulas_config = config.get_child("formulas");
    for( const auto&formula : formulas_config ){
      picture.formulas.emplace_back(formula.second.get<std::string>("") );
    }
  }catch (const std::exception&) {}
  // ratio-reference name
  picture.ratio_reference_title = config.get<std::string>("reference title", "");
  return picture;
};

Draw::Histogram2D GetHistogram2DConfig( const std::string& json_file ){
  boost::property_tree::ptree config;
  try {
    boost::property_tree::read_json(json_file, config);
  } catch( const std::exception& e  ){
    std::cout << "JsonConfig::GetHistogram2DConfig()" << std::endl;
    throw e;
  }
  Draw::Histogram2D histogram;

  auto histo_config = config.get_child( "histogram 2D" );
  histogram.file = histo_config.get<std::string>("file");
  histogram.name = histo_config.get<std::string>("name");
  histogram.is_log_z = histo_config.get<bool>("log z", false);
  histogram.is_unite = histo_config.get<bool>("unite", false);
  return histogram;
};

std::vector<Draw::Correlation>
GetCorrelationConfigs( const std::string& json_file, const std::string& branch_name ){
  boost::property_tree::ptree config;
  try {
    boost::property_tree::read_json(json_file, config);
  } catch( const std::exception& e  ){
    std::cout << "JsonConfig::GetCorrelationConfigs()" << std::endl;
    throw e;
  }
  std::vector<Draw::Correlation> correlations;
  auto corr_config = config.get_child(branch_name);
  for( const auto& conf : corr_config ){
    auto correlation_config = conf.second;
    correlations.emplace_back();
    try {
      auto names_config = correlation_config.get_child("names");
      for (const auto &name_config : names_config) {
        correlations.back().names.push_back(
            name_config.second.get<std::string>(""));
      }
    } catch (const std::exception& e) { throw e; }
    correlations.back().title = correlation_config.get<std::string>("title");
    correlations.back().file = correlation_config.get<std::string>("file");
    try {
      auto rebin_config = correlation_config.get_child("selection axes");
      for (const auto &axis : rebin_config) {
        auto title = axis.second.get<std::string>("name");
        auto n_bins = axis.second.get<int>("n bins");
        auto low_bin = axis.second.get<double>("low bin");
        auto up_bin = axis.second.get<double>("up bin");
        correlations.back().selection_axes.emplace_back(title, n_bins, low_bin,
                                                        up_bin);
      }
    }catch (const std::exception& e) {}
    correlations.back().scale = correlation_config.get<double>("scale", 1.0);
    correlations.back().projection_axis = correlation_config.get<std::string>("projection axis", "");
    correlations.back().marker = MarkerConstants::MARKERS.at(correlation_config.get<std::string>("marker"));
    correlations.back().color = MarkerConstants::COLORS.at(correlation_config.get<std::string>("color"));
  }
  return correlations;
};

Draw::Style GetStyleConfig( const std::string& json_file ){
  boost::property_tree::ptree config;
  try {
    boost::property_tree::read_json(json_file, config);
  } catch( const std::exception& e  ){
    std::cout << "JsonConfig::GetStyleConfig()" << std::endl;
    throw e;
  }
  Draw::Style style_config{};
  style_config.pad_left_margin = config.get<float>("pad left margin");
  style_config.pad_right_margin = config.get<float>("pad right margin");
  style_config.pad_bottom_margin = config.get<float>("pad bottom margin");
  style_config.legend_border_size = config.get<int>("legend border size");
  style_config.frame_line_width = config.get<int>("frame line width");
  style_config.marker_size = config.get<int>("marker size");
  style_config.line_width = config.get<int>("line width");
  try {
    auto title_size_config = config.get_child("title size");
    style_config.title_size.at(0) = title_size_config.get<float>("X");
    style_config.title_size.at(1) = title_size_config.get<float>("Y");
    style_config.title_size.at(2) = title_size_config.get<float>("Z");
  }catch (std::exception&) {}

  try {
    auto title_offset_config = config.get_child("title offset");
    style_config.title_offset.at(0) = title_offset_config.get<float>("X");
    style_config.title_offset.at(1) = title_offset_config.get<float>("Y");
    style_config.title_offset.at(2) = title_offset_config.get<float>("Z");
  }catch (std::exception&) {}

  try {
    auto label_offset_config = config.get_child("label offset");
    style_config.label_offset.at(0) = label_offset_config.get<float>("X");
    style_config.label_offset.at(1) = label_offset_config.get<float>("Y");
    style_config.label_offset.at(2) = label_offset_config.get<float>("Z");
  }catch (std::exception&) {}

  try {
    auto label_size_config = config.get_child("label size");
    style_config.label_size.at(0) = label_size_config.get<float>("X");
    style_config.label_size.at(1) = label_size_config.get<float>("Y");
    style_config.label_size.at(2) = label_size_config.get<float>("Z");
  }catch (std::exception&) {}

  return style_config;
}

}