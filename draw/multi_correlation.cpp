//
// Created by mikhail on 8/11/21.
//

#include "multi_correlation.h"

ClassImp(MultiCorrelation);

void MultiCorrelation::Scale(double scale) {
  for( auto &corr : correlations_ ){
    corr->Scale(scale);
  }
}
void MultiCorrelation::Rebin(const std::vector<Qn::AxisD> &axes) {
  for( auto &corr : correlations_ ){
    corr->Rebin(axes);
  }
}
void MultiCorrelation::Select(const std::vector<Qn::AxisD> &axes) {
  for( auto &corr : correlations_ ){
    corr->Select(axes);
  }
}
void MultiCorrelation::Project(const std::vector<std::string>& axes) {
  for( auto &corr : correlations_ ){
    corr->Project(axes);
  }
}
void MultiCorrelation::SetErrorOption(const std::string &error_option) {
  for( auto &corr : correlations_ ){
    corr->SetErrorOption(error_option);
  }
}
void MultiCorrelation::AddCorrelation(const std::string& file,
                                      const std::vector<std::string>& objects,
                                      const std::string& title,
                                      const std::vector<double>& weights_) {
  correlations_.push_back( std::make_unique<Correlation>( file, objects, title, weights_ ) );
}
