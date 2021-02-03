//
// Created by mikhail on 8/20/20.
//
#include "draw_functions.h"
#include <TStyle.h>

namespace Draw{

void DrawHistogram2D( const PictureConfig &picture_config, const Histogram2DConfig & histo ){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0),
                            picture_config.resolution.at(1));
  FileManager::Open(histo.file);
  auto histo2d = FileManager::GetObject<TH2F>( histo.name );
  try {
    std::string axes_title = ";"+ picture_config.axes_titles.at(0) + ";" +
                             picture_config.axes_titles.at(1);
    histo2d->SetTitle(axes_title.c_str());
  } catch (const std::exception&) {}

  histo2d->GetYaxis()->SetRangeUser(picture_config.y_axis_range.at(0),
                        picture_config.y_axis_range.at(1));
  histo2d->GetXaxis()->SetRangeUser(picture_config.x_axis_range.at(0),
                                 picture_config.x_axis_range.at(1));
  histo2d->SetMinimum(picture_config.z_axis_range.at(0));
  histo2d->SetMaximum(picture_config.z_axis_range.at(1));
  histo2d->GetZaxis()->SetTitle("");
  canvas->cd();
  gStyle->SetPalette(kRainBow);
  if( histo.is_unite )
    histo2d->Scale( 1./(double) histo2d->Integral("width") );
  histo2d->Draw("colz");
  TLatex* text{nullptr};
  for( size_t i=0; i<std::size(picture_config.texts); ++i ) {
    try {
    text = new TLatex(picture_config.text_positions.at(i).at(0),
                      picture_config.text_positions.at(i).at(1),
                      picture_config.texts.at(i).c_str());
    text->SetNDC();
    text->SetTextSize(picture_config.text_sizes.at(i));
    text->SetLineWidth(1);
    text->SetTextColor(kBlack);
    text->Draw("same");
    } catch (std::out_of_range&) {
      std::cout << "TLatex position set incorrect" << std::endl;
    }
  }
  std::vector<TF1*> formulas;
  int num=0;
  for( const auto& formula : picture_config.formulas ){
    std::string name = "formula_"+std::to_string(num);
    formulas.emplace_back( new TF1( name.c_str(), formula.c_str(), -100, 100 ) );
    formulas.back()->Draw("same");
    num++;
  }
  if( histo.is_log_z)
    gPad->SetLogz();
  canvas->SaveAs(picture_config.save_name.c_str());
};

void DrawHistograms1D(const PictureConfig &picture_config,
                      const std::vector<Histogram1DConfig>&histogram_configs,
                      const std::vector<Histogram1DConfig>&profile_configs){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0),
                            picture_config.resolution.at(1));
  auto histo_stack = new THStack( "histograms", "" );
  TLegend* legend;
  try{
    legend = new TLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                         picture_config.legend_position.at(2), picture_config.legend_position.at(3));
  } catch (std::out_of_range&) {
    legend = new TLegend();
  }
  try{
    std::string stack_title = ";" + picture_config.axes_titles.at(0) + ";" +
                              picture_config.axes_titles.at(1);
    histo_stack->SetTitle(stack_title.c_str());
  } catch (std::exception&) {}
  TH1* histo1d{nullptr};
  for( const auto &histo_config : histogram_configs ){
    FileManager::Open(histo_config.file);
    histo1d = FileManager::GetObject<TH1>(histo_config.name );
    histo1d->Scale(histo_config.scale);
    histo1d->SetTitle(histo_config.title.c_str());
    histo1d->SetMarkerStyle(histo_config.marker);
    histo1d->SetMarkerColor(histo_config.color);
    histo1d->SetLineColor(histo_config.color);
    histo1d->SetMarkerSize( gStyle->GetMarkerSize() );
    histo1d->SetLineWidth( gStyle->GetLineWidth() );
    histo_stack->Add(histo1d);
    legend->AddEntry(histo1d, histo1d->GetTitle(), "P");
  }
  TProfile* profile{nullptr};
  for( const auto &histo_config : profile_configs ){
    FileManager::Open(histo_config.file);
    profile = FileManager::GetObject<TProfile>(histo_config.name );
    profile->Scale(histo_config.scale);
    profile->SetTitle(histo_config.title.c_str());
    profile->SetMarkerStyle(histo_config.marker);
    profile->SetMarkerColor(histo_config.color);
    profile->SetLineColor(histo_config.color);
    profile->SetMarkerSize( gStyle->GetMarkerSize() );
    profile->SetLineWidth( gStyle->GetLineWidth() );
    histo_stack->Add(profile);
    legend->AddEntry(profile, profile->GetTitle(), "P");
  }
  canvas->cd();
  histo_stack->Draw();
  histo_stack->GetYaxis()->SetRangeUser(picture_config.y_axis_range.at(0),
                                        picture_config.y_axis_range.at(1));
  histo_stack->GetXaxis()->SetRangeUser(picture_config.x_axis_range.at(0),
                                        picture_config.x_axis_range.at(1));
//  histo_stack->Draw("E1+X0");
  histo_stack->Draw();
  legend->Draw("same");
  TLatex* text{nullptr};
  for( size_t i=0; i<std::size(picture_config.texts); ++i ) {
    try {
      text = new TLatex(picture_config.text_positions.at(i).at(0),
                        picture_config.text_positions.at(i).at(1),
                        picture_config.texts.at(i).c_str());
      text->SetNDC();
      text->SetTextSize(picture_config.text_sizes.at(i));
      text->SetLineWidth(1);
      text->SetTextColor(kBlack);
      text->Draw("same");
    } catch (std::out_of_range&) {
      std::cout << "TLatex position set incorrect" << std::endl;
    }
  }
  canvas->SaveAs(picture_config.save_name.c_str());
}

void Draw1D( const PictureConfig &picture_config, const std::vector<CorrelationConfig>&correlation_configs,
            const std::vector<GraphConfig>&graph_configs, const std::vector<Histogram1DConfig>&histogram_configs){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0), picture_config.resolution.at(1));
  std::string axes_title;
  try {
    axes_title = ";" + picture_config.axes_titles.at(0) + ";" +
                             picture_config.axes_titles.at(1);
  }catch (const std::exception&) {}
  auto* graph_stack = new TMultiGraph("graphs", axes_title.c_str());
  auto* histo_stack = new THStack( "histo", "" );
  TLegend* legend;
  try{
    legend = new TLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                         picture_config.legend_position.at(2), picture_config.legend_position.at(3));
  } catch (std::out_of_range&) {
    legend = new TLegend();
  }
  TFile* save_points_file{nullptr};
  if( picture_config.save_points ){
    save_points_file = TFile::Open( std::data(picture_config.save_name+".root"), "recreate" );
    save_points_file->cd();
  }
  for( const auto& config : correlation_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStatCalculate> containers;
    for( const auto& name : config.names ) {
      Qn::DataContainerStatCalculate container;
      try {
        container =
            Qn::DataContainerStatCalculate(*(FileManager::GetObject<Qn::DataContainerStatCollect>(name)));
      } catch (std::exception&) {
        container =
            *(FileManager::GetObject<Qn::DataContainerStatCalculate>(name));
      }
      for (const auto &axis : config.rebin_axes)
        container = container.Rebin(axis);
      for (const auto &axis : config.selection_axes)
        container = container.Select(axis);
      if( !std::empty(config.projection_axis) )
        container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }

    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    containers.at(0).SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
    auto graph = Qn::ToTGraph( containers.at(0) );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    if( save_points_file )
      graph->Write( config.title.c_str() );
    if( config.is_line ) {
      graph_stack->Add(graph, "L");
      legend->AddEntry(graph, graph->GetTitle(), "L");
      continue;
    }
    graph->SetMarkerStyle(config.marker);
    graph_stack->Add(graph);
    legend->AddEntry(graph, graph->GetTitle(), "P");
  }
  if( save_points_file )
    save_points_file->Close();
  for( const auto& config : graph_configs){
    FileManager::Open(config.file);
    auto graph = FileManager::GetObject<TGraph>(config.name);
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    graph_stack->Add(graph);
  }
  for( const auto& config : histogram_configs){
    FileManager::Open(config.file);
    auto histo = FileManager::GetObject<TH1>(config.name);
    histo->SetTitle(config.title.c_str());
    histo->SetLineColor(config.color);
    histo->SetMarkerColor(config.color);
    histo->SetMarkerStyle(config.marker);
    histo_stack->Add(histo);
  }
  canvas->cd();
  graph_stack->Draw("AP+E5");
  graph_stack->SetMinimum(picture_config.y_axis_range.at(0));
  graph_stack->SetMaximum(picture_config.y_axis_range.at(1));
  try {
    graph_stack->GetXaxis()->SetLimits(picture_config.x_axis_range.at(0),
                                       picture_config.x_axis_range.at(1));
  } catch (std::out_of_range&) {
    std::cout << "X axis range is set automatically" << std::endl;
  }
  histo_stack->Draw("same");
  legend->Draw();
//  try{
//    gPad->BuildLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
//                      picture_config.legend_position.at(2), picture_config.legend_position.at(3),
//                      "", "P");
//  } catch (std::out_of_range&) {
//    gPad->BuildLegend();
//  }
  auto line_zero = new TF1( "line", "0", -100, 100 );
  line_zero->Draw("same");
  TLatex* text{nullptr};
  for( size_t i=0; i<std::size(picture_config.texts); ++i ) {
    try {
      text = new TLatex(picture_config.text_positions.at(i).at(0),
                        picture_config.text_positions.at(i).at(1),
                        picture_config.texts.at(i).c_str());
      text->SetNDC();
      text->SetTextSize(picture_config.text_sizes.at(i));
      text->SetLineWidth(1);
      text->Draw("same");
    } catch (std::out_of_range&) {
      std::cout << "TLatex position set incorrect" << std::endl;
    }
  }
  canvas->SaveAs(picture_config.save_name.c_str());
};

void CompareCorrelations( const PictureConfig &picture_config, const std::vector<CorrelationConfig>& reference_configs,
                          const std::vector<CorrelationConfig>& compare_configs){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0), picture_config.resolution.at(1));
  std::string result_name;
  std::string ratio_name;
  try {
    result_name = ";;" + picture_config.axes_titles.at(1);
    ratio_name = ";"+picture_config.axes_titles.at(0) + ";"+ picture_config.axes_titles.at(2);
  }catch (const std::exception&) {}
  auto* result_stack = new TMultiGraph("results", result_name.c_str());
  auto* ratio_stack = new TMultiGraph("ratio", ratio_name.c_str());
  std::vector<Qn::DataContainerStatCalculate> references;
  TLegend* legend;
  try{
    legend = new TLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                         picture_config.legend_position.at(2), picture_config.legend_position.at(3));
  } catch (std::out_of_range&) {
    legend = new TLegend();
  }
  for( const auto& config : reference_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStatCalculate> containers;
    for( const auto& name : config.names ) {
      Qn::DataContainerStatCalculate container;
      try {
        container =
            Qn::DataContainerStatCalculate(*(FileManager::GetObject<Qn::DataContainerStatCollect>(name)));
      } catch (std::exception&) {
        container =
            *(FileManager::GetObject<Qn::DataContainerStatCalculate>(name));
      }
      for (const auto &axis : config.selection_axes)
        container = container.Select(axis);
      if( !std::empty(config.projection_axis) )
        container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }
    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    references.push_back(containers.at(0));
  }
  TFile* save_points_file{nullptr};
  if( picture_config.save_points ){
    save_points_file = TFile::Open( std::data(picture_config.save_name+".root"), "recreate" );
    save_points_file->cd();
  }
  for( size_t i=1; i<std::size(references); ++i ){
    references.at(0) = references.at(0) + references.at(i);
  }
  references.at(0) = references.at(0) * ( 1.0 / (double) std::size(references) );
  for( const auto& config : compare_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStatCalculate> containers;
    for( const auto& name : config.names ) {
      Qn::DataContainerStatCalculate container;
      try {
        container =
            Qn::DataContainerStatCalculate(*(FileManager::GetObject<Qn::DataContainerStatCollect>(name)));
      } catch (std::exception&) {
        container =
            *(FileManager::GetObject<Qn::DataContainerStatCalculate>(name));
      }
      for (const auto &axis : config.selection_axes)
        container = container.Select(axis);
      if( !std::empty(config.projection_axis) )
        container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }
    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    containers.at(0).SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
    auto ratio = containers.at(0) / references.at(0);
    ratio.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
    auto graph = Qn::ToTGraph( containers.at(0) );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    if( save_points_file ){
      save_points_file->cd();
      graph->Write(config.title.c_str());
    }
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    legend->AddEntry(graph, graph->GetTitle(),"P");
    result_stack->Add(graph);
    graph = Qn::ToTGraph( ratio );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    ratio_stack->Add(graph);
  }
  if( save_points_file )
    save_points_file->Close();

  references.at(0).SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
  auto graph = Qn::ToTGraph( references.at(0) );
  graph->SetTitle(picture_config.ratio_reference_title.c_str());
  if( !picture_config.is_reference_line ) {
    graph->SetLineColor(picture_config.reference_color);
    graph->SetMarkerColor(picture_config.reference_color);
    graph->SetMarkerStyle(picture_config.reference_marker);
    legend->AddEntry(graph, graph->GetTitle(),"P");
    result_stack->Add(graph);
  }else{
    graph->SetLineColor(picture_config.reference_color);
    legend->AddEntry(graph, graph->GetTitle(),"L");
    result_stack->Add(graph, "L");
  }
  canvas->cd();
  auto result_pad = new TPad("result", "result", 0.0, 0.35, 1.0, 1.0);
  auto ratio_pad = new TPad("ratio", "ratio", 0.0, 0.0, 1.0, .35);
  auto ratio_pad_scale = 1.0/0.35-0.3;
  auto result_pad_scale = 1.0/0.75;
  result_pad->cd();
  result_pad->SetBottomMargin(0);
  result_stack->Draw("AP+E5");
  result_stack->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("Y")*result_pad_scale, "Y");
  result_stack->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("Y")*result_pad_scale, "Y");
  result_stack->GetHistogram()->SetTitleOffset(gStyle->GetTitleOffset("Y")*result_pad_scale, "Y");
  result_stack->SetMinimum(picture_config.y_axis_range.at(0));
  result_stack->SetMaximum(picture_config.y_axis_range.at(1));
  result_stack->GetXaxis()->SetLimits(picture_config.x_axis_range.at(0),
                                     picture_config.x_axis_range.at(1));
  legend->Draw();
//  try{
////    gPad->BuildLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
////                      picture_config.legend_position.at(2), picture_config.legend_position.at(3),
////                      "", "P");
//  } catch (std::out_of_range&) {
//    gPad->BuildLegend();
//  }
  auto line_zero = new TF1( "zero", "0", -100, 100 );
  line_zero->SetLineColor(kBlue);
  line_zero->Draw("same");
  TLatex* text{nullptr};
  for( size_t i=0; i<std::size(picture_config.texts); ++i ) {
    try {
      text = new TLatex(picture_config.text_positions.at(i).at(0),
                        picture_config.text_positions.at(i).at(1),
                        picture_config.texts.at(i).c_str());
      text->SetNDC();
      text->SetTextSize(picture_config.text_sizes.at(i));
      text->SetLineWidth(1);
      text->Draw("same");
    } catch (std::out_of_range&) {
      std::cout << "TLatex position set incorrect" << std::endl;
    }
  }
  ratio_pad->cd();
  ratio_pad->SetTopMargin(0);
  ratio_pad->SetBottomMargin(gStyle->GetPadBottomMargin()*ratio_pad_scale);
  ratio_stack->Draw("AP+E5");
  ratio_stack->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("Y")* ratio_pad_scale, "Y");
  ratio_stack->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("X")* ratio_pad_scale, "X");
  ratio_stack->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("Y")* ratio_pad_scale, "Y");
  ratio_stack->GetHistogram()->SetTitleOffset(gStyle->GetTitleOffset("Y")* ratio_pad_scale, "Y");
  ratio_stack->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("X")* ratio_pad_scale, "X");
  ratio_stack->SetMinimum(picture_config.ratio_range.at(0));
  ratio_stack->SetMaximum(picture_config.ratio_range.at(1));
  ratio_stack->GetYaxis()->SetNdivisions(5,5, 0);
  ratio_stack->GetXaxis()->SetLimits(picture_config.x_axis_range.at(0),
                                      picture_config.x_axis_range.at(1));
  auto line_one = new TF1( "one", "1", -100, 100 );
  line_one->SetLineColor(picture_config.reference_color);
  line_one->Draw("same");
  line_zero->Draw("same");
  canvas->cd();
  result_pad->Draw();
  ratio_pad->Draw();
  canvas->Draw();
  canvas->SaveAs(picture_config.save_name.c_str());
};

void SetStyle(const StyleConfig & style){
  gStyle->SetPadLeftMargin(style.pad_left_margin);
  gStyle->SetPadRightMargin(style.pad_right_margin);
  gStyle->SetPadBottomMargin(style.pad_bottom_margin);
  gStyle->SetLegendBorderSize(style.legend_border_size);
  gStyle->SetFrameLineWidth(style.frame_line_width);
  gStyle->SetMarkerSize(style.marker_size);
  gStyle->SetLineWidth(style.line_width);

  gStyle->SetTitleSize(style.title_size.at(0), "X");
  gStyle->SetTitleSize(style.title_size.at(1), "Y");
  gStyle->SetTitleSize(style.title_size.at(2), "Z");

  gStyle->SetLabelSize(style.label_size.at(0), "X");
  gStyle->SetLabelSize(style.label_size.at(1), "Y");
  gStyle->SetLabelSize(style.label_size.at(2), "Z");

  gStyle->SetLabelOffset(style.label_offset.at(0), "X");
  gStyle->SetLabelOffset(style.label_offset.at(1), "Y");
  gStyle->SetLabelOffset(style.label_offset.at(2), "Z");

  gStyle->SetTitleOffset(style.title_offset.at(0), "X");
  gStyle->SetTitleOffset(style.title_offset.at(1), "Y");
  gStyle->SetTitleOffset(style.title_offset.at(2), "Z");

  gStyle->SetOptStat(0);
};

}