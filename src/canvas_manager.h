//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CANVAS_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_CANVAS_MANAGER_H_

#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <TCanvas.h>
#include <THStack.h>
#include <TLine.h>
#include <TMultiGraph.h>
#include <TStyle.h>
class CanvasManager {
public:
  static CanvasManager* Instance(){
    if(!instance_)
      instance_ = new CanvasManager();
    return instance_;
  }
  void SetResolution(const ushort& width, const ushort& height){
    resolution_.at(0) = width;
    resolution_.at(1) = height;
  }
  void ResetCanvas(std::string_view name){
    canvas_ = std::make_unique<TCanvas>( name.data(), "",
                              resolution_.at(0), resolution_.at(1) );
  }
  void SaveCanvas(std::string_view name){
    if( !canvas_ ){
      std::cout << "CanvasManager::SaveCanvas: canvas is not set" << std::endl;
      abort();
    }
    canvas_->SaveAs( name.data() );
  }
  void AddToCanvas(TMultiGraph* graph_stack){
    graph_stack_ = graph_stack;
  }
  void AddToCanvas(THStack* histo_stack){
    histo_stack_ = histo_stack;
  }
  void Draw(){
    if( !canvas_ ){
      std::cout << "CanvasManager::Draw: canvas is not set" << std::endl;
      abort();
    }
//    style_->cd();
    canvas_->cd();
    if( !graph_stack_ ){
      std::cout << "CanvasManager::Draw: graphs are not set" << std::endl;
      abort();
    }
    std::cout << histo_stack_ << std::endl;

    auto line = new TF1("zero", "0", -10.0, 10.0);
    auto y_cm = new TLine(-0.74, -0.25, -0.74, 0.25);
    graph_stack_->Draw("AP");
    if( histo_stack_ )
      histo_stack_->Draw("same+NOSTACK");
    gPad->BuildLegend();
    line->Draw("same");
    y_cm->Draw("same");
  }
private:

  void ReadStyleConfig( const std::string& file_path="../src/config/style.json" ){
    boost::property_tree::ptree config;
    boost::property_tree::read_json(file_path, config);
//    gStyle = new TStyle("style", "");
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
  CanvasManager(){
      ReadStyleConfig();
  };
  ~CanvasManager() = default;
  static CanvasManager* instance_;
  std::array<ushort, 2> resolution_{1000, 1100};
  std::unique_ptr<TCanvas> canvas_{nullptr};
  TStyle* style_{nullptr};
  TMultiGraph* graph_stack_{nullptr};
  THStack* histo_stack_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_CANVAS_MANAGER_H_
