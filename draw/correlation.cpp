//
// Created by mikhail on 2/28/21.
//

#include "correlation.h"

Correlation::Correlation(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title,
                         const std::vector<double>& weights)
    : DrawableObject(file_name, objects, title) {
  std::vector<Qn::DataContainerStatCalculate> containers;
  for( const auto& name : objects ){
    try {
      containers.emplace_back(
          *(this->ReadObjectFromFile<Qn::DataContainerStatCalculate>(name)));
    } catch (std::exception&) {
      containers.emplace_back(
          *(this->ReadObjectFromFile<Qn::DataContainerStatCollect>(name)));
    }
  }
  if( !weights.empty() ) {
    int idx=0;
    for (auto &container: containers) {
      try{ container = container * weights.at(idx); }catch( const std::out_of_range& e){
        std::string error_msg = std::string(__func__ ) + ": The vector of weights is shorter than the vector of averaging Qn::DataContainer's";
        throw std::out_of_range(error_msg);
      }
      idx++;
    }
  }
  average_ = containers.front();
  for( size_t i=1; i< containers.size(); ++i )
    average_ = average_ + containers.at(i);
  average_ = average_ / (double)containers.size();
  combinations_ = containers;
}
Correlation::~Correlation() {}

Correlation operator/( const Correlation& num, const Correlation& den){
  Correlation result;
  result.title_ = num.title_+"_ratio";
  result.marker_ = num.marker_;
  result.color_ = num.color_;
  auto num_container = num.average_;
  auto den_container = den.average_;
  auto res_container = num_container / den_container;
  result.average_ = res_container;

  return result;
}

void Correlation::RefreshPoints() {
  average_.SetErrors(error_type_);
  points_ = Qn::ToTGraph(average_);
  for( int i=0; i<points_->GetN(); ++i ){
    auto y_err = points_->GetErrorY(i);
    if( isnan( y_err ) )
      points_->SetPointError(i, 0., 0.);
  }
  if( fit_ )
    points_->Fit(fit_);
  if( calculate_systematics_from_variation_ ){
    std::vector<Qn::DataContainerStatCalculate> variations;
    for( const auto& combination : combinations_ ){
      variations.emplace_back( average_ - combination );
    }
    sys_error_points_ = Qn::ToTGraph( average_ );
    for( int i=0; i<sys_error_points_->GetN(); ++i ){
      auto x_hi = average_.GetAxes().front().GetUpperBinEdge(i);
      auto x_lo = average_.GetAxes().front().GetLowerBinEdge(i);
      auto x_err = (x_hi - x_lo) / 4;
      double y_err=0;
      for( auto var : variations ){
        auto err = fabs(var.At(i).Mean());
        if( y_err < err )
          y_err=err;
//        y_err += var.At(i).Mean()*var.At(i).Mean();
      }
//      y_err= sqrt(y_err);
      sys_error_points_->SetPointError(i, x_err, y_err);
    }
  }
  this->SetMarkerStyle();
}