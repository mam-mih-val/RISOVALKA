//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_
#define FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_

#include "readable_object.h"
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TObject.h>

#include <utility>

class DrawableObject {
public:
  DrawableObject() = default;
  DrawableObject(const DrawableObject& );
  DrawableObject(const std::string &file_name,
                 const std::vector<std::string> &objects,
                 std::string title)
      : title_(std::move(title)) {}
  virtual ~DrawableObject() = default;

  bool IsDrawn(){ return static_cast<bool>(points_); }
  [[nodiscard]] int GetColor() const { return color_; }
  void Fit( TF1* function, std::vector<float> range = {} ){
    this->RefreshPoints();
    if( range.size() < 2 )
      points_->Fit(function);
    else
      points_->Fit(function, "", "", range.at(0), range.at(1));
    fit_.reset(dynamic_cast<TF1*>(points_->GetListOfFunctions()->Last()));
    if(fit_)
      fit_->SetLineColor(color_);
  }
  virtual void RefreshPoints() {}
  TGraphErrors* GetPoints() {
    this->RefreshPoints();
    return points_.get();
  }
  TGraphErrors* ReleasePoints() {
    this->RefreshPoints();
    return points_.release();
  }
  [[nodiscard]] TF1 *GetFit() const { return fit_.get(); }
  [[nodiscard]] TGraphErrors *GetSysErrorPoints() const { return sys_error_points_.get(); }
  [[nodiscard]] TGraphErrors *ReleaseSysErrorPoints() { return sys_error_points_.release(); }
  void SetStyle( int color, int marker ){ color_=color; marker_=marker; }
  [[nodiscard]] bool IsLine() const{ return marker_ < 0; }
  void SavePoints(){ points_->Write(); }
  std::string GetTitle() { return title_;}
  void SetTitle(const std::string &title) { title_ = title; }
  [[nodiscard]] const std::string &GetErrorOption() const { return error_option_; }
  void SetErrorOption(const std::string &error_option) {
    error_option_ = error_option;
  }
  void SetSysErrors( double x_error, double relative_sys_error );
  void SetSysErrors( double x_error, std::vector<double> systematical_errors );
  void SetSysErrors( std::vector<double> x_errors, std::vector<double> y_errors );

protected:
  void SetMarkerStyle();
  int color_{kBlack};
  int marker_{kFullCircle};
  std::unique_ptr<TF1> fit_{nullptr};
  std::unique_ptr<TGraphErrors> points_{nullptr};
  std::unique_ptr<TGraphErrors> sys_error_points_{nullptr};
  std::string error_option_{"Z"};
  std::string title_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_DRAWABLE_OBJECT_H_
