//
// Created by mikhail on 2/28/21.
//

#include "graph.h"
#include <TGraphAsymmErrors.h>
ClassImp(Graph);

Graph::Graph( DrawableObject* other ) : DrawableObject(*other) {
  points_ = new TGraphErrors( other->GetPoints()->GetN() );
  points_ = new TGraphErrors( other->GetPoints()->GetN() );
  points_->SetTitle( title_.c_str() );
  for( int i=0; i<other->GetPoints()->GetN(); i++ ){
    auto x = other->GetPoints()->GetPointX(i);
    auto y = other->GetPoints()->GetPointY(i);
    auto x_err = other->GetPoints()->GetErrorX(i);
    auto y_err = other->GetPoints()->GetErrorY(i);
    points_->SetPoint(i, x, y);
    points_->SetPointError(i, x_err, y_err);
  }
  if( other->GetSysErrorPoints() ){
    sys_error_points_ = new TGraphErrors( other->GetSysErrorPoints()->GetN() );
    sys_error_points_ = new TGraphErrors( other->GetSysErrorPoints()->GetN() );
    sys_error_points_->SetTitle( title_.c_str() );
    for( int i=0; i<other->GetSysErrorPoints()->GetN(); i++ ){
      auto x = other->GetSysErrorPoints()->GetPointX(i);
      auto y = other->GetSysErrorPoints()->GetPointY(i);
      auto x_err = other->GetSysErrorPoints()->GetErrorX(i);
      auto y_err = other->GetSysErrorPoints()->GetErrorY(i);
      sys_error_points_->SetPoint(i, x, y);
      sys_error_points_->SetPointError(i, x_err, y_err);
    }
  }
};

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
  if ( x_axis.size() < (size_t) points_->GetN() ){
    auto points_old = points_;
    points_ = new TGraphErrors( x_axis.size() );
    for( int i=0; i<points_->GetN(); i++ ){
      auto x = x_axis.at(i);
      auto y = points_old->GetPointY(i);
      auto x_err = points_old->GetErrorX(i);
      auto y_err = points_old->GetErrorY(i);
      points_->SetPoint(i, x, y);
      points_->SetPointError(i, x_err, y_err);
    }
    delete points_old;
  }
}


Graph::Graph(const std::string &file_name,
             const std::vector<std::string> &objects, const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TGraphErrors*> graphs;
  graphs.reserve(objects.size());
  for( const auto& name : objects ){
    try {
      graphs.push_back(this->ReadObjectFromFile<TGraphErrors>(name));
    } catch (std::exception&) {
      graphs.push_back((TGraphErrors*)this->ReadObjectFromFile<TGraphAsymmErrors>(name));
    }
  }
  points_ = new TGraphErrors(graphs.front()->GetN() );
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
Graph::~Graph() {}

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
