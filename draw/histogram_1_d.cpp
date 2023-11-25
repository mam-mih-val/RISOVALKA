//
// Created by mikhail on 2/28/21.
//

#include <RtypesCore.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <limits>
#include "histogram_1_d.h"

Histogram1D::Histogram1D(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title)
    : DrawableObject(file_name, objects, title) {
  std::vector<TH1*> histograms;
  for( const auto& name : objects ){
    try {
      histograms.push_back(FileManager::ReadObject<TH1>(file_name, name));
    } catch (std::exception&) {
      histograms.push_back(dynamic_cast<TH1*>(FileManager::ReadObject<TProfile>(file_name, name)));
    }
  }
  histogram_= std::unique_ptr<TH1>( dynamic_cast<TH1*>(histograms.front()->Clone()));
  for (size_t i=1; i<histograms.size(); ++i) {
    histogram_->Add( histograms.at(i) );
  }
  histogram_->Scale( 1.0 / (double) histograms.size() );

}

Histogram1D::~Histogram1D() {}

void Histogram1D::RefreshPoints() {
  std::vector<double> x_axis{};
  std::vector<double> y_axis{};
  std::vector<double> y_err_axis{};
  x_axis.reserve( histogram_->GetNbinsX() );
  y_axis.reserve( histogram_->GetNbinsX() );
  y_err_axis.reserve( histogram_->GetNbinsX() );
  for( int i=0; i<histogram_->GetNbinsX(); ++i ){
    auto x = histogram_->GetBinCenter(i+1);
    auto y = histogram_->GetBinContent(i+1);
    auto y_err = histogram_->GetBinError(i+1);
    if( abs(y) <= std::numeric_limits<double>::epsilon() )
      continue;
    x_axis.push_back(x);
    y_axis.push_back(y);
    y_err_axis.push_back(y_err);
  }
    points_.reset( new TGraphErrors(x_axis.size(), x_axis.data(), y_axis.data(), nullptr, y_err_axis.data()) );
  this->SetMarkerStyle();
}

Histogram1D operator/( const Histogram1D& num, const Histogram1D& den){
  Histogram1D result;
  result.title_ = num.title_+"_ratio";
  result.marker_ = num.marker_;
  result.color_ = num.color_;

  const auto& num_histo = num.histogram_;
  const auto& den_histo = den.histogram_;
  auto res_histo = std::unique_ptr<TH1>( dynamic_cast<TH1F*>(num_histo->Clone( result.title_.c_str() )) );
  res_histo->Divide( den_histo.get() );
  result.histogram_ = std::move(res_histo);
  return result;
}

Histogram1D::Histogram1D(const Histogram1D &other) : DrawableObject(other) {
  histogram_ = std::unique_ptr<TH1>( dynamic_cast< TH1* >( other.histogram_->Clone() ) );
}
