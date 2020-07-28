//
// Created by mikhail on 7/17/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CONFIG_READER_H_
#define FLOW_DRAWING_TOOLS_SRC_CONFIG_READER_H_

#include "correlation.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class ConfigReader {
public:
  ConfigReader() = default;
  virtual ~ConfigReader() = default;
  void Open(const std::string& file){
    boost::property_tree::ptree config;
    boost::property_tree::read_json(file, config);
    try {
      config_ = config.get_child("correlations");
    }catch (const std::exception&) {
      std::cout << "ConfigReader::Open: no correlations found" << std::endl;
    }
  }
  [[nodiscard]] std::vector<Correlation> Read(){
    std::vector<Correlation> correlations;
    for( const auto& config : config_  ){
      correlations.push_back(ReadSingleCorrelation(config.second));
    }
    return correlations;
  }
  Correlation ReadSingleCorrelation(const boost::property_tree::ptree& config);

protected:
  boost::property_tree::ptree config_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_CONFIG_READER_H_
