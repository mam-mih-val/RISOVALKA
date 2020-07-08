//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_MANAGER_H_

#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <DataContainer.h>
#include <Stats.h>

#include "canvas_manager.h"
#include "file_manager.h"
#include "marker_constants.h"

class CorrelationManager {
public:
  static CorrelationManager* Instance(){
    if(!instance_)
      instance_ = new CorrelationManager;
    return instance_;
  }
  void Draw(const std::string& path_to_file);
private:
  CorrelationManager() : file_manager_( FileManager::Instance() ),
                         canvas_manager_(CanvasManager::Instance()){};
  ~CorrelationManager() = default;
  void AddCorrelation(boost::property_tree::ptree config);
  void ReadConfig(boost::property_tree::ptree config);
  void ReadGraphs(const boost::property_tree::ptree& config);
  void AddGraph(const boost::property_tree::ptree& config);
  static CorrelationManager* instance_;
  FileManager* file_manager_{nullptr};
  CanvasManager* canvas_manager_{nullptr};
  std::vector<Qn::DataContainer<Qn::Stats>> containers_;
  TMultiGraph* graph_stack_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_MANAGER_H_
