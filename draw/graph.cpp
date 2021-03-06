//
// Created by mikhail on 2/28/21.
//

#include "graph.hpp"
ClassImp(Graph);

Graph::Graph(const std::string &file_name,
             const std::vector<std::string> &objects, const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TGraphErrors*> graphs;
  graphs.reserve(objects.size());
  for( const auto& name : objects ){
    graphs.push_back( this->ReadObjectFromFile<TGraphErrors>(name) );
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
void Graph::RefreshPoints() { DrawableObject::RefreshPoints(); }
Graph::~Graph() {}
