//
// Created by mikhail on 5/7/21.
//

#include "double_differential_correlation.h"
#include <iomanip>

void DoubleDifferentialCorrelation::Calculate() {
  this->Rebin({projection_axis_, slice_axis_});
  for( size_t slice_bin=0; slice_bin <slice_axis_.size(); slice_bin++ ){
    std::string name{ slice_axis_.Name() };
    auto lo = slice_axis_.GetLowerBinEdge(slice_bin);
    auto hi = slice_axis_.GetUpperBinEdge(slice_bin);
    auto proj_container = correlation_.Select( {name, 1, lo, hi} );
    proj_container = proj_container.Projection({projection_axis_.Name()});
    std::string graph_name{ name+"_"+std::to_string(lo)+"-"+std::to_string(hi) };
    proj_container.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
    projection_points_.emplace_back( Qn::ToTGraph( proj_container ) );
    projection_points_.back()->SetName( graph_name.c_str() );
    std::ostringstream stream_lo;
    stream_lo << std::setprecision(2) << lo;
    std::ostringstream stream_hi;
    stream_hi << std::setprecision(2) << hi;
    std::string title{ slice_variable_name_+" " + stream_lo.str()+"-"+stream_hi.str()+" "+slice_variable_units_ };
    projection_points_.back()->SetTitle(title.c_str());
    projections_.emplace_back( new Graph );
    projections_.back()->SetPoints( projection_points_.back() );
    projections_.back()->SetTitle(projection_points_.back()->GetTitle());
  }
  this->ColorObjects( projections_ );
}
void DoubleDifferentialCorrelation::SaveToFile(const std::string &file_name) {
  auto file_out = TFile::Open( file_name.c_str(), "recreate" );
  file_out->cd();
  for (auto  prj : projection_points_)
    prj->Write();
  file_out->cd("/");
  file_out->mkdir("slopes");
  file_out->cd("slopes");
}
void DoubleDifferentialCorrelation::SetErrorOption(
    const std::string &error_option) {
  error_option_ = error_option;
}
