//
// Created by mikhail on 11/9/21.
//

#include "double_differential_histogram.h"
void DoubleDifferentialHistogram::Projection(const std::string &axis,
                                             int first_bin, int last_bin) {
  TH1D* histogram_projection{nullptr};
  for( int i=first_bin; i<=last_bin; i++ ){
    auto proj_name = "proj_"+axis+std::to_string(i)+"_"+std::string (histo2d_->GetName());
    std::string proj_title;
    double lo=0;
    double hi=0;
    if( axis == "X" || axis == "x" ){
      histogram_projection = dynamic_cast<TH1D*>(histo2d_->ProjectionX( proj_name.c_str(),i, i ));
      hi = histo2d_->GetYaxis()->GetBinUpEdge( i );
      lo = histo2d_->GetYaxis()->GetBinLowEdge( i );
    }
    else if( axis == "Y" || axis == "y" ){
      histogram_projection = dynamic_cast<TH1D*>(histo2d_->ProjectionY( proj_name.c_str(),i, i ));
      hi = histo2d_->GetXaxis()->GetBinUpEdge( i );
      lo = histo2d_->GetXaxis()->GetBinLowEdge( i );
    }
    else
      throw std::runtime_error("Unknown axis. Please use X or Y to set the axis");
    projections_.push_back( new Histogram1D() );
    projections_.back()->SetHistogram( histogram_projection );
    std::ostringstream stream_lo;
    stream_lo << std::setprecision(2) << lo;
    std::ostringstream stream_hi;
    stream_hi << std::setprecision(2) << hi;
    std::string title{ stream_lo.str()+"<"+ slice_variable_name_ +"<"+stream_hi.str()+", "+slice_variable_units_ };
    projections_.back()->SetTitle( title );
  }
  this->ColorObjects();
}
void DoubleDifferentialHistogram::ColorObjects() {
  std::vector<int> colors;
  if( projections_.size() < palette_.size() ){
    auto bias = palette_.size() / projections_.size();
    for( size_t i=0; i<projections_.size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    for( size_t i=0; i<projections_.size(); ++i ) {
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
  for( auto projection : projections_ ){
    projection->SetStyle(colors.at(i), marker_);
    ++i;
  }
}
