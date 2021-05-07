//
// Created by mikhail on 5/6/21.
//

#include "dv1dy_container.h"
#include <iomanip>
ClassImp(dv1dy_container);

void dv1dy_container::Calculate( const Qn::AxisD& remaining_axis, const Qn::AxisD& slice_axis, const Qn::AxisD& rapidity_axis ){
  this->Rebin({rapidity_axis, remaining_axis, slice_axis});
  for( size_t slice_bin=0; slice_bin <slice_axis.size(); slice_bin++ ){
    std::string name{ slice_axis.Name() };
    auto lo = slice_axis.GetLowerBinEdge(slice_bin);
    auto hi = slice_axis.GetUpperBinEdge(slice_bin);
    slope_graphs_.push_back( new TGraphErrors( remaining_axis.size() ) );
    offset_graphs_.push_back( new TGraphErrors( remaining_axis.size() ) );
    std::string graph_name{ "dv1dy_"+name+"_"+std::to_string(lo)+"-"+std::to_string(hi) };
    slope_graphs_.back()->SetName(graph_name.c_str());
    graph_name = "offset_"+name+"_"+std::to_string(lo)+"-"+std::to_string(hi);
    offset_graphs_.back()->SetName(graph_name.c_str());
    std::ostringstream stream_lo;
    stream_lo << std::setprecision(2) << lo;
    std::ostringstream stream_hi;
    stream_hi << std::setprecision(2) << hi;
    std::string title{ stream_lo.str()+"<"+ slice_variable_name_ +"<"+stream_hi.str()+", "+slice_variable_units_ };
    slope_graphs_.back()->SetTitle(title.c_str());
    offset_graphs_.back()->SetTitle(title.c_str());
    for( size_t argument_bin=0; argument_bin < remaining_axis.size(); argument_bin++ ){
      std::string arg_name{ remaining_axis.Name() };
      auto arg_lo = remaining_axis.GetLowerBinEdge(argument_bin);
      auto arg_hi = remaining_axis.GetUpperBinEdge(argument_bin);
      auto argument = remaining_axis.GetBinCenter(argument_bin);
      auto proj = correlation_.Rebin( { name, 1, lo, hi } );
      proj = proj.Rebin({arg_name, 1, arg_lo, arg_hi});
      proj = proj.Projection( { rapidity_axis.Name() } );
      projections_.push_back( Qn::ToTGraph( proj ) );
      graph_name = name+"_"+std::to_string(lo)+"-"+std::to_string(hi)+"_"+
          arg_name+"_"+std::to_string(arg_lo)+"-"+std::to_string(arg_hi);
      projections_.back()->SetName( graph_name.c_str() );
      graph_name="fit_"+graph_name;
      auto fit = new TF1( graph_name.c_str(), "pol1", rapidity_axis.GetFirstBinEdge(), rapidity_axis.GetLastBinEdge() );
      projections_.back()->Fit(fit);
      auto slope = fit->GetParameter(1);
      auto slope_err = fit->GetParError(1);
      auto offset = fit->GetParameter(0);
      auto offset_err = fit->GetParError(0);
      slope_graphs_.back()->SetPoint( argument_bin, argument, slope );
      slope_graphs_.back()->SetPointError( argument_bin, 0, slope_err );
      offset_graphs_.back()->SetPoint( argument_bin, argument, offset );
      offset_graphs_.back()->SetPointError( argument_bin, 0, offset_err );
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
  file_out->cd("/");
  file_out->mkdir("slopes");
  file_out->cd("slopes");
  for (auto slope : slope_graphs_)
    slope->Write();
  file_out->cd("/");
  file_out->mkdir("offset");
  file_out->cd("offset");
  for (auto offset : offset_graphs_)
    offset->Write();
  file_out->Close();
}
void dv1dy_container::FillGraphs() {
  std::vector<int> colors;
  if( slope_graphs_.size() < palette_.size() ){
    auto bias = palette_.size() / slope_graphs_.size();
    for( int i=0; i<slope_graphs_.size(); ++i )
      colors.push_back(palette_.at(i*bias));
  }else{
    auto bias=0;
    auto position=0;
    for( int i=0; i<slope_graphs_.size(); ++i ) {
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
  for( auto graph : slope_graphs_ ){
    slopes_.push_back( new Graph );
    slopes_.back()->SetPoints( graph );
    slopes_.back()->SetTitle(graph->GetTitle());
    slopes_.back()->SetStyle(colors.at(i), marker_);
    graph->SetLineColor(colors.at(i));
    graph->SetMarkerColor(colors.at(i));
    graph->SetMarkerColor(marker_);
    ++i;
  }
  i=0;
  for( auto graph : offset_graphs_ ){
    offsets_.push_back( new Graph );
    offsets_.back()->SetPoints( graph );
    offsets_.back()->SetTitle(graph->GetTitle());
    offsets_.back()->SetStyle(colors.at(i), marker_);
    graph->SetLineColor(colors.at(i));
    graph->SetMarkerColor(colors.at(i));
    graph->SetMarkerColor(marker_);
    ++i;
  }
}
void dv1dy_container::SetPalette(const std::vector<int> &palette) {
  palette_ = palette;
}
