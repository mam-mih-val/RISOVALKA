//
// Created by mikhail on 2/28/21.
//

#include <TProfile.h>
#include "histogram_1_d.h"

ClassImp(Histogram1D);

Histogram1D::Histogram1D(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TH1*> histograms;
  for( const auto& name : objects ){
    try {
      histograms.push_back(this->ReadObjectFromFile<TH1>(name));
    } catch (std::exception&) {
      histograms.push_back(dynamic_cast<TH1*>(this->ReadObjectFromFile<TProfile>(name)));
    }
  }
  histogram_ = (TH1*) histograms.front()->Clone();
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

Histogram1D operator/( const Histogram1D& num, const Histogram1D& den){
  Histogram1D result;
  result.title_ = num.title_+"_ratio";
  result.marker_ = num.marker_;
  result.color_ = num.color_;
  auto num_histo = num.histogram_;
  auto den_histo = den.histogram_;
  auto res_histo = dynamic_cast<TH1F*>(num_histo->Clone( result.title_.c_str() ));
  res_histo->Divide( den_histo );
  result.histogram_ = res_histo;
  return result;
}