//
// Created by mikhail on 2/28/21.
//

#include "histogram_1_d.hpp"
ClassImp(Histogram1D);

Histogram1D::Histogram1D(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TH1F*> histograms;
  for( const auto& name : objects ){
    histograms.push_back( this->ReadObjectFromFile<TH1F>(name) );
  }
  histogram_ = (TH1F*) histograms.front()->Clone();
  for (size_t i=1; i<histograms.size(); ++i) {
    histogram_->Add( histograms.at(i) );
  }
  histogram_->Scale( 1.0 / (double) histograms.size() );

}

Histogram1D::~Histogram1D() {}

void Histogram1D::RefreshPoints() {
  if( !points_ )
    points_ = new TGraphErrors( histogram_->GetNbinsX() );
  for( int i=0; i<histogram_->GetNbinsX(); ++i ){
    auto x = histogram_->GetBinCenter(i+1);
    auto y = histogram_->GetBinContent(i+1);
    auto y_err = histogram_->GetBinError(i+1);
    points_->SetPoint(i, x, y);
    points_->SetPointError(i, 0, y_err);
  }
  this->SetMarkerStyle();
}
