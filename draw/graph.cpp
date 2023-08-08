//
// Created by mikhail on 2/28/21.
//

#include "graph.h"
#include <TGraphAsymmErrors.h>

void Graph::RecalculateXaxis( const std::vector<double>& x_axis ){
  if( x_axis.size() >= (size_t) points_->GetN() ){
    for( int i=0; i<points_->GetN(); i++ ){
      auto x = x_axis.at(i);
      auto y = points_->GetPointY(i);
      auto x_err = points_->GetErrorX(i);
      auto y_err = points_->GetErrorY(i);
      points_->SetPoint(i, x, y);
      points_->SetPointError(i, x_err, y_err);
    }
  }
}

Graph::Graph(const std::string &file_name,
             const std::vector<std::string> &objects, const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TGraphErrors*> graphs;
  graphs.reserve(objects.size());
  for( const auto& name : objects ){
    try {
      graphs.push_back( FileManager::ReadObject<TGraphErrors>(file_name, name));
    } catch (std::exception&) {
      graphs.push_back( dynamic_cast<TGraphErrors*>( FileManager::ReadObject<TGraphAsymmErrors>(file_name, name) ));
    }
  }
  points_ = std::make_unique<TGraphErrors>(graphs.front()->GetN());
  for( int i=0; i < graphs.front()->GetN(); ++i ){
    double x = graphs.front()->GetPointX(i);
    double y = 0;
    double xerr=0;
    double yerr=0;
    for( auto graph : graphs){
      y+=graph->GetPointY(i);
      yerr+=graph->GetErrorY(i)*graph->GetErrorY(i);
      xerr+=graph->GetErrorX(i)*graph->GetErrorX(i);
    }
    y/= (double) graphs.size();
    yerr/= (double) graphs.size();
    yerr = sqrt(yerr);
    xerr/= (double) graphs.size();
    xerr= sqrt(xerr);
    points_->SetPoint(i, x, y);
    points_->SetPointError(i, xerr, yerr);
  }
  this->SetMarkerStyle();
}
void Graph::RefreshPoints() { this->SetMarkerStyle(); }

void Graph::Scale( double s ){
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX( i );
    auto y = points_->GetPointY( i );
    auto x_err = points_->GetErrorX( i );
    auto y_err = points_->GetErrorY( i );
    points_->SetPoint(i, x, y*s);
    points_->SetPointError(i, x_err, y_err*s);
  }
  if( sys_error_points_ ){
    for( int i=0; i<sys_error_points_->GetN(); ++i ){
      auto x = sys_error_points_->GetPointX( i );
      auto y = sys_error_points_->GetPointY( i );
      auto x_err = sys_error_points_->GetErrorX( i );
      auto y_err = sys_error_points_->GetErrorY( i );
      sys_error_points_->SetPoint(i, x, y*s);
      sys_error_points_->SetPointError(i, x_err, y_err*s);
    }
  }
}

Graph::Graph(const std::string &file_name, const std::string &title, const std::string &format) {
  title_ = title;
  points_ = std::make_unique<TGraphErrors>(file_name.c_str(), format.c_str() );
  points_->SetTitle(title.c_str());
}

void Graph::ScaleXaxis(double scale) {
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX( i );
    auto y = points_->GetPointY( i );
    auto x_err = points_->GetErrorX( i );
    auto y_err = points_->GetErrorY( i );
    points_->SetPoint(i, x*scale, y);
    points_->SetPointError(i, x_err, y_err);
  }
  if( sys_error_points_ ){
    for( int i=0; i<sys_error_points_->GetN(); ++i ){
      auto x = sys_error_points_->GetPointX( i );
      auto y = sys_error_points_->GetPointY( i );
      auto x_err = sys_error_points_->GetErrorX( i );
      auto y_err = sys_error_points_->GetErrorY( i );
      sys_error_points_->SetPoint(i, x*scale, y);
      sys_error_points_->SetPointError(i, x_err, y_err);
    }
  }
}

void Graph::TranslateXaxis(double translation) {
  for( int i=0; i<points_->GetN(); ++i ){
    auto x = points_->GetPointX( i );
    auto y = points_->GetPointY( i );
    auto x_err = points_->GetErrorX( i );
    auto y_err = points_->GetErrorY( i );
    points_->SetPoint(i, x+translation, y);
    points_->SetPointError(i, x_err, y_err);
  }
  if( sys_error_points_ ){
    for( int i=0; i<sys_error_points_->GetN(); ++i ){
      auto x = sys_error_points_->GetPointX( i );
      auto y = sys_error_points_->GetPointY( i );
      auto x_err = sys_error_points_->GetErrorX( i );
      auto y_err = sys_error_points_->GetErrorY( i );
      sys_error_points_->SetPoint(i, x+translation, y);
      sys_error_points_->SetPointError(i, x_err, y_err);
    }
  }
}
