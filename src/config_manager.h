//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CONFIG_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_CONFIG_MANAGER_H_

#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <DataContainer.h>
#include <Stats.h>
#include <THStack.h>
#include <TProfile.h>

#include "canvas_manager.h"
#include "file_manager.h"
#include "marker_constants.h"

class ConfigManager {
public:
  static ConfigManager * Instance(){
    if(!instance_)
      instance_ = new ConfigManager;
    return instance_;
  }
  void Draw(const std::string& path_to_file);
private:
  ConfigManager() : file_manager_( FileManager::Instance() ),
                         canvas_manager_(CanvasManager::Instance()){};
  ~ConfigManager() = default;
  void AddCorrelation(boost::property_tree::ptree config);
  void ReadConfig(boost::property_tree::ptree config);
  void ReadGraphs(const boost::property_tree::ptree& config);
  void ReadHistos(const boost::property_tree::ptree& config);
  void AddGraph(const boost::property_tree::ptree& config);
  void AddHisto(const boost::property_tree::ptree& config);
  static ConfigManager * instance_;
  FileManager* file_manager_{nullptr};
  CanvasManager* canvas_manager_{nullptr};
  std::vector<Qn::DataContainer<Qn::Stats>> containers_;
  TMultiGraph* graph_stack_{nullptr};
  THStack* histo_stack_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_CONFIG_MANAGER_H_
