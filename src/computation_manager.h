//
// Created by mikhail on 7/8/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "file_manager.h"
#include "flow_method.h"
#include "rnd_not_ext_method.h"

class ComputationManager {
public:
  ComputationManager() = default;
  ~ComputationManager() = default;
  void Compute(const std::string& path_to_file){
    boost::property_tree::read_json(path_to_file, config_);
    file_manager_->Open(config_.get<std::string>("file"));
    file_out_ = TFile::Open( (config_.get<std::string>("save name")).c_str(),
                            "recreate");
    auto methods = config_.get_child("methods");
    for( const auto& method : methods ){
      ReadMethod(method.second);
    }
    file_out_->Close();

  }
private:
  void ReadMethod(const boost::property_tree::ptree& method){
    FlowMethod* flow_method;
    std::vector<Qn::DataContainerStats*> qn_qn;
    if ( method.get<std::string>("method") == "RND NO EXTRAPOLATION" ){
      flow_method = new RndNotExtMethod();
    }

    flow_method->SetName( method.get<std::string>("name") );
    flow_method->SetUnQn( file_manager_->GetObject<Qn::DataContainerStats>(
        method.get<std::string>("observed")) );
    auto qn_qn_names = method.get_child("resolution");
    for( const auto& corr : qn_qn_names ){
      auto qn_name = corr.second.get<std::string>("");
      qn_qn.push_back( file_manager_->GetObject<Qn::DataContainerStats>( qn_name ) );
    }
    flow_method->SetQnQn(qn_qn);
    flow_method->ComputeResolution();
    flow_method->ComputeFlow();
    flow_method->Write(file_out_);
  }

  boost::property_tree::ptree config_;
  FileManager* file_manager_{FileManager::Instance()};
  TFile* file_out_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_COMPUTATION_MANAGER_H_
