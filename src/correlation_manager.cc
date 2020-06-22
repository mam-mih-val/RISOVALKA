//
// Created by mikhail on 6/21/20.
//

#include "correlation_manager.h"

CorrelationManager* CorrelationManager::instance_= nullptr;

void CorrelationManager::Draw(const std::string& path_to_file){
  boost::property_tree::ptree config;
  boost::property_tree::read_json(path_to_file, config);
  ReadConfig(config);
  canvas_manager_->AddToCanvas(graph_stack_);
  canvas_manager_->ResetCanvas("canv");
  canvas_manager_->Draw();
  canvas_manager_->SaveCanvas(config.get<std::string>("save name"));
}

void CorrelationManager::ReadConfig(boost::property_tree::ptree config){
  file_manager_->SetFile(config.get<std::string>("file"));
  auto axis_config = config.get_child("axis title");
  std::string stack_title = ";"+ axis_config.get<std::string>("X")
      +";"+axis_config.get<std::string>("Y");
  graph_stack_ = new TMultiGraph("stack", stack_title.data());
  auto corr_config = config.get_child("correlations");
  for( const auto& conf : corr_config ){
    AddCorrelation(conf.second);
  }
  graph_stack_->Draw();
  auto axis_range_conf = config.get_child("axis range");
  graph_stack_->SetMinimum(axis_range_conf.get<double>("min"));
  graph_stack_->SetMaximum(axis_range_conf.get<double>("max"));
}

void CorrelationManager::AddCorrelation(boost::property_tree::ptree config){
  auto corr = file_manager_->GetObject<Qn::DataContainerStats>(config.get<std::string>("name"));
  auto rebin_config = config.get_child("rebin axis");
  for( const auto& axis : rebin_config ){
    auto title = axis.second.get<std::string>("name");
    auto n_bins = axis.second.get<int>("n bins");
    auto low_bin = axis.second.get<double>("low bin");
    auto up_bin = axis.second.get<double>("up bin");
    corr = corr.Rebin({title, n_bins, low_bin, up_bin});
  }
  corr = corr.Projection({config.get<std::string>("projection axis")});
  corr = corr*( config.get<double>("scale") );
  corr.SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
  auto graph = Qn::DataContainerHelper::ToTGraph(corr);
  graph->SetTitle( (config.get<std::string>("title")).data() );
  graph->SetMarkerStyle( MarkerConstants::MARKERS.at(
      config.get<std::string>("marker")
      ) );
  graph->SetMarkerColor( MarkerConstants::COLORS.at(
      config.get<std::string>("color")
      ) );
  graph->SetLineColor( MarkerConstants::COLORS.at(
      config.get<std::string>("color")
      ) );
  graph_stack_->Add(graph);
}