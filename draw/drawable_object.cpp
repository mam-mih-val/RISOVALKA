//
// Created by mikhail on 2/28/21.
//

#include "drawable_object.h"
ClassImp(DrawableObject);

void DrawableObject::SetMarkerStyle() {
  points_->SetTitle(title_.c_str());
  if( marker_ > 0 )
    points_->SetMarkerStyle(marker_);
  else
    points_->SetLineStyle(-marker_);
  points_->SetMarkerColor(color_);
  points_->SetLineColor(color_);
  points_->SetFillColorAlpha(color_, 0.1);
  if( fit_ )
    fit_->SetLineColor(color_);
  if( sys_error_points_ ){
    sys_error_points_->SetTitle( std::data("sys_"+title_) );
    if( marker_ > 0 )
      sys_error_points_->SetMarkerStyle(marker_);
    else
      sys_error_points_->SetLineStyle(-marker_);
    sys_error_points_->SetMarkerColor(color_);
    sys_error_points_->SetLineColor(color_);
    sys_error_points_->SetFillColorAlpha(color_, 0.1);
  }
}

void DrawableObject::SetSysErrors( double x_error, double relative_sys_error ){
  this->RefreshPoints();
  sys_error_points_ = new TGraphErrors( points_->GetN() );
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX(i);
    auto y = points_->GetPointY(i);
    auto y_err = points_->GetPointY(i)*relative_sys_error;
    sys_error_points_->SetPoint(i, x, y);
    sys_error_points_->SetPointError(i, x_error, y_err);
  }
}

void DrawableObject::SetSysErrors( double x_error, std::vector<double> systematical_errors ){
  this->RefreshPoints();
  sys_error_points_ = new TGraphErrors( points_->GetN() );
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX(i);
    auto y = points_->GetPointY(i);
    auto y_err = systematical_errors.at(i);
    sys_error_points_->SetPoint(i, x, y);
    sys_error_points_->SetPoint(i, x_error, y_err);
  }
}

void DrawableObject::SetSysErrors( std::vector<double> x_errors, std::vector<double> y_errors ){
  this->RefreshPoints();
  sys_error_points_ = new TGraphErrors( points_->GetN() );
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX(i);
    auto y = points_->GetPointY(i);
    auto x_err = x_errors.at(i);
    auto y_err = y_errors.at(i);
    sys_error_points_->SetPoint(i, x, y);
    sys_error_points_->SetPoint(i, x_err, y_err);
  }
}
