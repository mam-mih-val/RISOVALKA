//
// Created by mikhail on 5/6/21.
//

#include "dv1dy_container.h"
#include <iomanip>
ClassImp(dv1dy_container);

void dv1dy_container::Calculate( const Qn::AxisD& remaining_axis, const Qn::AxisD& slice_axis, const Qn::AxisD& rapidity_axis ){
  this->Rebin({remaining_axis, slice_axis});
  auto test_function = new TF1( "test", formula_.c_str() );
  const auto n_par = test_function->GetNumberFreeParameters();
  delete test_function;
  for( int i=0; i< n_par; ++i  ){
    fit_par_points_.emplace_back();
  }
  for( size_t slice_bin=0; slice_bin <slice_axis.size(); slice_bin++ ){
    std::string name{ slice_axis.Name() };
    auto lo = slice_axis.GetLowerBinEdge(slice_bin);
    auto hi = slice_axis.GetUpperBinEdge(slice_bin);
    for( int i=0; i<n_par; i++ ){
      fit_par_points_.at(i).push_back( new TGraphErrors( remaining_axis.size() ) );
      std::string graph_name =  "p"+std::to_string(i)+"_"+name+"_"+std::to_string( (int) lo)+"_"+std::to_string((int)hi);
      fit_par_points_.at(i).back()->SetName(graph_name.c_str());
      std::ostringstream stream_lo;
      stream_lo << std::setprecision(2) << lo;
      std::ostringstream stream_hi;
      stream_hi << std::setprecision(2) << hi;
      std::string graph_title{ stream_lo.str()+"<"+ slice_variable_name_ +"<"+stream_hi.str()+", "+slice_variable_units_ };
      fit_par_points_.at(i).back()->SetTitle(graph_title.c_str());
    }
    for( size_t argument_bin=0; argument_bin < remaining_axis.size(); argument_bin++ ){
      std::string arg_name{ remaining_axis.Name() };
      auto arg_lo = remaining_axis.GetLowerBinEdge(argument_bin);
      auto arg_hi = remaining_axis.GetUpperBinEdge(argument_bin);
      auto argument = remaining_axis.GetBinCenter(argument_bin);
      auto proj = correlation_.Rebin( { name, 1, lo, hi } );
      proj = proj.Rebin({arg_name, 1, arg_lo, arg_hi});
      proj = proj.Projection( { rapidity_axis.Name() } );
      proj.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
      auto zero_bin = proj.GetAxes().front().FindBin(0.0);
      if( zero_bin < 0 )
        throw std::runtime_error( "There is no bin containing midrapidity y=0" );
      auto true_offset = proj.At(zero_bin).Mean();
      auto offset_error = proj.At(zero_bin).StandardErrorOfMean();
      proj = proj.Rebin(rapidity_axis);
      projections_.push_back( Qn::ToTGraph( proj ) );
      std::string proj_graph_name = name+"_"+std::to_string(lo)+"_"+std::to_string(hi)+"_"+
          arg_name+"_"+std::to_string(arg_lo)+"_"+std::to_string(arg_hi);
      projections_.back()->SetName( proj_graph_name.c_str() );
      std::string fit_name="fit_"+proj_graph_name;
      auto fit = new TF1( fit_name.c_str(), formula_.c_str(), rapidity_axis.GetFirstBinEdge(), rapidity_axis.GetLastBinEdge() );
      if( is_true_offset_ )
        fit->FixParameter(0, true_offset);
      for( auto pair : fixed_parameters_ ){
        fit->FixParameter( pair.first, pair.second );
      }
      projections_.back()->Fit(fit);
      std::vector<double> parameter_val_;
      std::vector<double> parameter_err_;
      for( int i=0; i<n_par; ++i ){
        auto arg_val = fit->GetParameter(i);
        auto arg_err = fit->GetParError(i);
        fit_par_points_.at(i).back()->SetPoint( argument_bin, argument, arg_val );
        fit_par_points_.at(i).back()->SetPointError( argument_bin, 0, arg_err );
        if( is_true_offset_ && i==0 ) {
          fit_par_points_.at(i).back()->SetPoint(argument_bin, argument,
                                                      true_offset);
          fit_par_points_.at(i).back()->SetPointError(argument_bin, 0,
                                                      offset_error);
        }
      }
    }
  }
  FillGraphs();
}

void dv1dy_container::SaveToFile( const std::string& file_name ){
  auto file_out = TFile::Open( file_name.c_str(), "recreate" );
  file_out->mkdir("projections");
  file_out->cd("projections");
  for (auto  prj : projections_)
    prj->Write();
  int i=0;
  for( auto graphs : fit_par_points_){
    file_out->cd("/");
    std::string dir_name = "p"+std::to_string(i);
    file_out->mkdir( dir_name.c_str() );
    ++i;
    for(auto graph : graphs ){
      graph->Write();
    }
  }
  file_out->Close();
}
void dv1dy_container::FillGraphs() {
  std::vector<int> colors;
  if(fit_par_points_.front().size() < palette_.size() ){
    auto bias = palette_.size() / fit_par_points_.front().size();
    for( size_t i=0; i< fit_par_points_.front().size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    for( size_t i=0; i< fit_par_points_.front().size(); ++i ) {
      if( i<palette_.size() ) {
        colors.push_back(palette_.at(position+bias));
        position++;
      } else{
        position=0;
        bias+=3;
      }
    }
  }
  while( markers_.size() < fit_par_points_.front().size() ){
    markers_.push_back( 20+markers_.size() );
  }
  int marker_num=0;
  for( const auto& graphs : fit_par_points_){
    int i=0;
    fit_par_graphs_.emplace_back();
    for( auto graph : graphs ){
      fit_par_graphs_.back().push_back( new Graph );
      fit_par_graphs_.back().back()->SetPoints( graph );
      fit_par_graphs_.back().back()->SetTitle(graph->GetTitle());
      fit_par_graphs_.back().back()->SetStyle(colors.at(i), markers_.at(marker_num));
      ++i;
    }
    marker_num++;
  }
}
void dv1dy_container::SetPalette(const std::vector<int> &palette) {
  palette_ = palette;
}
