//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
#define FLOW_DRAWING_TOOLS_SRC_PICTURE_H_

#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TAxis.h>

#include "drawable_object.h"
#include <utility>
class Picture {
public:
  Picture(std::string name, const std::array<int, 2> &resolution)
      : name_(std::move(name)), resolution_(resolution) {
    canvas_ =  new TCanvas( name_.c_str(), "", resolution.at(0), resolution.at(1) );
    auto stack_name = name_+"_stack";
    stack_ =  new TMultiGraph(stack_name.c_str(), "");
  }
  void AddDrawable( DrawableObject* obj ){
    assert(obj);
    drawable_objects_.push_back(obj);
  }
  void SetAxisTitles(const std::vector<std::string> &axis_titles) {
    assert(axis_titles.size()==2);
    axis_titles_ = axis_titles;
    auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1);
    stack_->SetTitle( title.c_str() );
  }
  void SetXRange(const std::array<float, 2> &x_range) { x_range_ = x_range; }
  void SetYRange(const std::array<float, 2> &y_range) { y_range_ = y_range; }
  void Draw(){
    for( auto obj : drawable_objects_ ){
      stack_->Add( obj->GetPoints() );
    }
    canvas_->cd();
    stack_->Draw("AP");
    stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
    stack_->GetYaxis()->SetLimits(y_range_.at(0), y_range_.at(1));
    stack_->Draw();
  }
  void Save( const std::string& type ){
    assert(canvas_);
    auto save_name = name_+"."+type;
    canvas_->SaveAs( save_name.c_str() );
  }
  void SaveAs( const std::string& name, const std::string& type ){
    assert(canvas_);
    auto save_name = name+"."+type;
    canvas_->SaveAs( save_name.c_str() );
  }

protected:
  std::string name_;
  std::array<int, 2> resolution_;
  TCanvas* canvas_;
  TMultiGraph* stack_;
  std::vector<std::string> axis_titles_;
  std::array<float, 2> x_range_;
  std::array<float, 2> y_range_;
  std::vector<DrawableObject*> drawable_objects_;
  ClassDef(Picture, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
