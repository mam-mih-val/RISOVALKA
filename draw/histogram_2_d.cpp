//
// Created by mikhail on 3/31/21.
//

#include "histogram_2_d.h"

ClassImp(Histogram2D);

Histogram2D::Histogram2D(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title){
  file_name_ = file_name;
  title_ = title;
  std::vector<TH2*> histograms;
  histograms.reserve(objects.size());
  for( const auto& name : objects ){
    histograms.push_back( this->ReadObjectFromFile<TH2>(name) );
  }
  histo2d_ = dynamic_cast<TH2*>(histograms.front()->Clone());
  for (size_t i=1; i<histograms.size(); ++i) {
    histo2d_->Add( histograms.at(i) );
  }
  histo2d_->Scale( 1.0 / (double) histograms.size() );
}
Histogram2D::~Histogram2D() = default;
void Histogram2D::RefreshPoints() {
  if( !histogram_ )
    throw std::runtime_error( "Projection of 2D along 1 axis is not set" );
  Histogram1D::RefreshPoints();
}
void Histogram2D::Projection(const std::string& axis, int first_bin, int last_bin) {
  auto proj_name = "proj_"+axis+std::to_string(first_bin)+"-"+std::to_string(last_bin)+"_"+std::string (histo2d_->GetName());
  if( axis == "X" || axis == "x" ){
    histogram_ = dynamic_cast<TH1F*>(histo2d_->ProjectionX( proj_name.c_str(),first_bin, last_bin ));
  }
  else if( axis == "Y" || axis == "y" ){
    auto histo = histo2d_->ProjectionX( proj_name.c_str(),first_bin, last_bin );
    histogram_ = (TH1F*) (histo2d_->ProjectionY( proj_name.c_str(),first_bin, last_bin ));
  }
  else
    throw std::runtime_error("Unknown axis. Please use X or Y to set the axis");
}
