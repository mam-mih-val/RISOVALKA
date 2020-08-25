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
#include <TLatex.h>
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
  void AddText(const std::string& text){
    assert( !std::empty(text) );
    if( !text_)
      text_ = new TLatex( 0.25, 0.85, text.c_str() );
    text_->SetNDC();
    text_->SetTextSize(0.04);
    text_->SetLineWidth(1);;
  }
  void SetLegendPosition(const std::vector<double> &legend_position){
      legend_position_ = legend_position;
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
  void AddRatios(TMultiGraph* ratio_stack){
    ratio_stack_ = ratio_stack_;
  }
  void AddToCanvas(THStack* histo_stack){
    histo_stack_ = histo_stack;
  }
  void Draw();
  void DrawWithRatios();
private:

  void ReadStyleConfig( const std::string& file_path="../src/config/style.json" );
  CanvasManager(){
      ReadStyleConfig();
  };
  ~CanvasManager() = default;
  static CanvasManager* instance_;
  std::array<ushort, 2> resolution_{1000, 1100};
  std::unique_ptr<TCanvas> canvas_{nullptr};
  TMultiGraph* graph_stack_{nullptr};
  TMultiGraph* ratio_stack_{nullptr};
  THStack* histo_stack_{nullptr};
  std::vector<double> legend_position_;
  TLatex* text_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_CANVAS_MANAGER_H_
