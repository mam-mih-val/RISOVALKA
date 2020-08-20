//
// Created by mikhail on 6/21/20.
//

#include "canvas_manager.h"

CanvasManager* CanvasManager::instance_= nullptr;

void CanvasManager::ReadStyleConfig( const std::string& file_path ){
  boost::property_tree::ptree config;
  boost::property_tree::read_json(file_path, config);
  gStyle->SetPadLeftMargin(config.get<float>("pad left margin") );
  gStyle->SetPadRightMargin(config.get<float>("pad right margin") );
  gStyle->SetPadBottomMargin(config.get<float>("pad bottom margin") );
  gStyle->SetLegendBorderSize(config.get<int>("legend border size"));
  gStyle->SetFrameLineWidth(config.get<int>("frame line width"));
  gStyle->SetMarkerSize(config.get<int>("marker size"));
  gStyle->SetLineWidth(config.get<int>("line width"));
  auto title_size_config = config.get_child("title size");
  gStyle->SetTitleSize(title_size_config.get<float>("X"),"X");
  gStyle->SetTitleSize(title_size_config.get<float>("Y"),"Y");

  auto title_offset_config = config.get_child("title offset");
  gStyle->SetTitleSize(title_offset_config.get<float>("X"),"X");
  gStyle->SetTitleSize(title_offset_config.get<float>("Y"),"Y");

  gStyle->SetOptStat(0);
}

void CanvasManager::Draw(){
  assert(canvas_);
  canvas_->cd();
  assert(graph_stack_);
  [[maybe_unused]] auto line = new TF1("zero", "0", -10.0, 100.0);
  graph_stack_->Draw("AP");
  if( histo_stack_ )
    histo_stack_->Draw("same+NOSTACK");
  try{
    gPad->BuildLegend(legend_position_.at(0), legend_position_.at(1),
                      legend_position_.at(2), legend_position_.at(3),
                      "", "P");
  } catch (std::out_of_range&) {
    gPad->BuildLegend();
  }
  if(text_) {
    text_->Draw();
  }
  line->Draw("same");
}