//
// Created by mikhail on 7/8/20.
//

#include "computation_manager.h"

void ComputationManager::Init(const std::string& path_to_file){
  boost::property_tree::read_json(path_to_file, config_);
  file_manager_->Open(config_.get<std::string>("file"));
  corr_stack_.SetConfig(path_to_file);
  corr_stack_.FillStack();
  file_out_ = TFile::Open( (config_.get<std::string>("save name")).c_str(),
                           "recreate");
  auto methods = config_.get_child("methods");
  for( const auto& method : methods ){
    InitMethod(method.second);
  }
}

void ComputationManager::Compute(){
  for( const auto& method : methods_ ){
    method->ComputeResolution();
    method->ComputeFlow();
    method->Write(file_out_);
  }
  file_out_->Close();
}

void ComputationManager::InitMethod(const boost::property_tree::ptree& method){
  auto un_qn_name = method.get<std::string>("observed");
  std::vector<std::string> qn_qn_names;
  auto resolution_config = method.get_child("resolution");
  for( const auto& corr : resolution_config)
    qn_qn_names.push_back(corr.second.get<std::string>(""));
  std::vector<std::string> components_names;
  auto components_config = method.get_child("components");
  for( const auto&component : components_config)
    components_names.push_back(component.second.get<std::string>(""));

  for( const auto& component_name : components_names ){
    if ( method.get<std::string>("method") == "RND NO EXTRAPOLATION" ){
      methods_.push_back(new RndNotExtMethod());
    }else if ( method.get<std::string>("method") == "THREE SUB METHOD" ){
      methods_.push_back(new TreeSubMethod());
    }else{
      std::cout << "ComputationManager::ReadMethod: unknown method "
                << method.get<std::string>("method") << std::endl;
      abort();
    }
    methods_.back()->SetName( method.get<std::string>("name") + "_" + component_name );
    auto un_qn_correlation = corr_stack_.GetCorrelation(un_qn_name, component_name);
    methods_.back()->SetUnQn(un_qn_correlation);
    auto qn_qn_correlations = corr_stack_.GetVectorOfCorrelations(qn_qn_names, component_name);
    methods_.back()->SetQnQn(qn_qn_correlations);
  }
//
//  methods_.back()->ComputeResolution();
//  methods_.back()->ComputeFlow();
//  methods_.back()->Write(file_out_);
}