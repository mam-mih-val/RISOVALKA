//
// Created by mikhail on 8/11/21.
//

#include "multi_correlation.h"

ClassImp(MultiCorrelation);

void MultiCorrelation::ApplyStyle() {
  std::vector<int> colors;
  if( correlations_.size() < palette_.size() ){
    auto bias = palette_.size() / correlations_.size();
    for( size_t i=0; i<correlations_.size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    for( size_t i=0; i<correlations_.size(); ++i ) {
      if( i<palette_.size() ) {
        colors.push_back(palette_.at(position+bias));
        position++;
      } else{
        position=0;
        bias+=3;
      }
    }
  }
  int i=0;
  for( auto correlation : correlations_ ){
    if( markers_.size() == 1 ) {
      correlation->SetStyle(colors.at(i), markers_.front());
    }else{
      correlation->SetStyle(colors.at(i), markers_.at(i));
    }
    ++i;
  }
}
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
                                      const std::string& title) {
  correlations_.push_back( new Correlation( file, objects, title ) );
}
