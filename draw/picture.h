//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
#define FLOW_DRAWING_TOOLS_SRC_PICTURE_H_

#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TObject.h>
#include <TLegend.h>

#include "drawable_object.h"
#include <TLatex.h>
#include <utility>

class Picture : public TObject {
public:
  Picture() = default;
  ~Picture() override = default;
  Picture(std::string name, const std::array<int, 2> &resolution)
      : name_(std::move(name)), resolution_(resolution) {
    canvas_ =  new TCanvas( name_.c_str(), "", resolution.at(0), resolution.at(1) );
    canvas_->SetBatch(true);
    std::string stack_name = name_+"_stack";
    stack_ =  new TMultiGraph(stack_name.c_str(), "");
    zero_line_ = new TF1( "zero_line", "0", -100, 100 );
  }
  [[nodiscard]] TCanvas *GetCanvas() const { return canvas_; }
  virtual void SetAxisTitles(const std::vector<std::string> &axis_titles) {}
  void SetXRange(const std::array<float, 2> &x_range) { x_range_ = x_range; }
  void SetYRange(const std::array<float, 2> &y_range) { y_range_ = y_range; }
  virtual void Draw() {};
  void Save( const std::string& type ){
    assert(canvas_);
    auto save_name = name_+"."+type;
    canvas_->SaveAs( save_name.c_str() );
  }
  void SavePoints(const std::string& name, const std::string& type){
    assert(stack_);
    auto save_name = name+"."+type;
    auto file_out = TFile::Open( save_name.c_str(), "recreate" );
    TGraph* graph;
    for( int i=0; i < stack_->GetListOfGraphs()->GetSize(); ++i ){
      graph = dynamic_cast<TGraph*>(stack_->GetListOfGraphs()->At(i));
      graph->Write( graph->GetTitle() );
    }
//    stack_->SaveAs( save_name.c_str() );
    file_out->Close();
  }
  void Save( const std::string& name, const std::string& type ){
    assert(canvas_);
    auto save_name = name+"."+type;
    canvas_->SaveAs( save_name.c_str() );
  }
  TF1 *GetZeroLine() const { return zero_line_; }
  void DrawZeroLine(bool draw_zero_line) {
    Picture::draw_zero_line = draw_zero_line;
  }
  void SetAutoLegend(bool auto_legend) { auto_legend_ = auto_legend; }
  void AddText( TLatex text, float size=0.04 ){ texts_.push_back(new TLatex(text)); text_sizes_.push_back(size); }
  void AddLegend( TLegend* legend ){ legends_.push_back( legend ); auto_legend_=false; }
  void AddFunction( TF1* function ){ functions_.push_back(function); }
  void SetLogY(bool is_log_y=true) { Picture::is_log_y = is_log_y; }
  void SetLogX(bool is_log_x=true) { Picture::is_log_x = is_log_x; }
  void SetLogZ(bool is_log_z=true) { Picture::is_log_z = is_log_z; }

protected:
  std::string name_;
  std::array<int, 2> resolution_;
  TCanvas* canvas_;
  TMultiGraph* stack_;
  std::vector<TF1*> functions_;
  std::vector<std::string> axis_titles_;
  TF1* zero_line_{nullptr};
  bool draw_zero_line{true};
  std::array<float, 2> x_range_;
  std::array<float, 2> y_range_;
  std::vector<TLatex*> texts_;
  std::vector<float> text_sizes_;
  std::vector<TLegend*> legends_;
  bool is_log_y{false};
  bool is_log_x{false};
  bool is_log_z{false};
  bool auto_legend_{true};
  ClassDefOverride(Picture, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
