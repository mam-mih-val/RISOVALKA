//
// Created by mikhail on 7/8/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "correlation_stack.h"
#include "file_manager.h"
#include "flow_method.h"
#include "rnd_not_ext_method.h"
#include "tree_sub_method.h"

class ComputationManager {
public:
  ComputationManager() = default;
  ~ComputationManager() = default;
  void Init(const std::string& path_to_file);
  void Compute();

private:
  void InitMethod(const boost::property_tree::ptree& method);
  CorrelationStack corr_stack_;
  boost::property_tree::ptree config_;
  FileManager* file_manager_{FileManager::Instance()};
  TFile* file_out_;
  std::vector<FlowMethod*> methods_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_
