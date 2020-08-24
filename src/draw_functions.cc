//
// Created by mikhail on 8/20/20.
//
#include "draw_functions.h"
#include <TStyle.h>

namespace Draw{

void DrawHistogram2D( const Picture&picture_config, const Histogram2D& histo ){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0),
                            picture_config.resolution.at(1));
  auto text = new TLatex(picture_config.text_position.at(0),
                         picture_config.text_position.at(1),
                         picture_config.text.c_str());
  FileManager::Open(histo.file);
  auto histo2d = FileManager::GetObject<TH2F>( histo.name );
  histo2d->SetTitle(picture_config.axis_titles.c_str());
  histo2d->SetMinimum(picture_config.y_axis_range.at(0));
  histo2d->SetMaximum(picture_config.y_axis_range.at(1));
  histo2d->GetXaxis()->SetLimits(picture_config.x_axis_range.at(0),
                                 picture_config.x_axis_range.at(1));
  canvas->cd();
  histo2d->Draw("colz");
  text->Draw("same");
  canvas->SaveAs(picture_config.save_name.c_str());
};

void Draw1D( const Picture&picture_config, const std::vector<Correlation>&correlation_configs,
            const std::vector<Graph>&graph_configs, const std::vector<Histogram1D>&histogram_configs){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0), picture_config.resolution.at(1));
  TLatex* text{nullptr};
  try {
    text = new TLatex(picture_config.text_position.at(0),picture_config.text_position.at(1),
                           picture_config.text.c_str());
    text->SetNDC();
    text->SetTextSize(0.04);
    text->SetLineWidth(1);
  } catch (std::out_of_range&) {
    std::cout << "TLatex position set incorrect" << std::endl;
  }
  auto* graph_stack = new TMultiGraph("graphs", picture_config.axis_titles.c_str());
  auto* histo_stack = new THStack( "histo", "" );

  for( const auto& config : correlation_configs){
    FileManager::Open(config.file);
    auto container = *(FileManager::GetObject<Qn::DataContainerStats>(config.name));
    for(const auto& axis : config.rebin_axes)
      container = container.Rebin(axis);
    container = container.Projection({config.projection_axis});
    container = container* config.scale;
    container.SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
    auto graph = Qn::ToTGraph( container );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    graph_stack->Add(graph);
  }
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
  try{
    gPad->BuildLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                      picture_config.legend_position.at(2), picture_config.legend_position.at(3),
                      "", "P");
  } catch (std::out_of_range&) {
    gPad->BuildLegend();
  }
  auto line_zero = new TF1( "line", "0", -100, 100 );
  line_zero->Draw("same");
//  if( text )
    text->Draw();
  canvas->SaveAs(picture_config.save_name.c_str());
};

void CompareCorrelations( const Picture&picture_config, const std::vector<Correlation>& reference_configs,
                          const std::vector<Correlation>& compare_configs){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0), picture_config.resolution.at(1));

  TLatex* text{nullptr};
  try {
    text = new TLatex(picture_config.text_position.at(0),picture_config.text_position.at(1),
                      picture_config.text.c_str());
    text->SetNDC();
    text->SetTextSize(0.04);
    text->SetLineWidth(1);
  } catch (std::out_of_range&) {
    std::cout << "TLatex position set incorrect" << std::endl;
  }
  auto* result_stack = new TMultiGraph("results", picture_config.axis_titles.c_str());
  auto* ratio_stack = new TMultiGraph("ratio", picture_config.axis_titles.c_str());
  std::vector<Qn::DataContainerStats> references;
  for( const auto& config : reference_configs){
    FileManager::Open(config.file);
    auto container = *(FileManager::GetObject<Qn::DataContainerStats>(config.name));
    for(const auto& axis : config.rebin_axes)
      container = container.Rebin(axis);
    container = container.Projection({config.projection_axis});
    container = container* config.scale;
    references.push_back(container);
  }
  for( size_t i=1; i<std::size(references); ++i ){
    references.at(0) = references.at(0) + references.at(i);
  }
  for( const auto& config : compare_configs){
    FileManager::Open(config.file);
    auto container = *(FileManager::GetObject<Qn::DataContainerStats>(config.name));
    for(const auto& axis : config.rebin_axes)
      container = container.Rebin(axis);
    container = container.Projection({config.projection_axis});
    container = container* config.scale;
    auto ratio = container / references.at(0);
    auto graph = Qn::ToTGraph( container );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    result_stack->Add(graph);
    graph = Qn::ToTGraph( ratio );
    graph->SetTitle(config.title.c_str());
    graph->SetLineColor(config.color);
    graph->SetMarkerColor(config.color);
    graph->SetMarkerStyle(config.marker);
    ratio_stack->Add(graph);
  }
  canvas->cd();
  auto result_pad = new TPad("result", "result", 0.0, 0.35, 1.0, 1.0);
  auto ratio_pad = new TPad("ratio", "ratio", 0.0, 0.0, 1.0, .35);
  result_pad->cd();
  result_pad->SetBottomMargin(0);
  result_stack->Draw("AP+E5");
  result_stack->GetHistogram()->SetLabelSize(0.035, "Y");
  result_stack->SetMinimum(picture_config.y_axis_range.at(0));
  result_stack->SetMaximum(picture_config.y_axis_range.at(1));
  result_stack->GetXaxis()->SetLimits(picture_config.x_axis_range.at(0),
                                     picture_config.x_axis_range.at(1));
  try{
    gPad->BuildLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                      picture_config.legend_position.at(2), picture_config.legend_position.at(3),
                      "", "P");
  } catch (std::out_of_range&) {
    gPad->BuildLegend();
  }
  text->Draw();
  ratio_pad->cd();
  ratio_pad->SetTopMargin(0);
  ratio_pad->SetBottomMargin(0.25);
  ratio_stack->Draw("AP+E5");
  ratio_stack->GetHistogram()->SetLabelSize(0.065, "X");
  ratio_stack->GetHistogram()->SetLabelSize(0.065, "Y");
  canvas->cd();
  result_pad->Draw();
  ratio_pad->Draw();
  canvas->Draw();
  canvas->SaveAs(picture_config.save_name.c_str());
};

void SetStyle(const Style& style){
  gStyle->SetPadLeftMargin(style.pad_left_margin);
  gStyle->SetPadRightMargin(style.pad_right_margin);
  gStyle->SetPadBottomMargin(style.pad_bottom_margin);
  gStyle->SetLegendBorderSize(style.legend_border_size);
  gStyle->SetFrameLineWidth(style.frame_line_width);
  gStyle->SetMarkerSize(style.marker_size);
  gStyle->SetLineWidth(style.line_width);
  gStyle->SetTitleSize(style.title_size.at(0), "X");
  gStyle->SetTitleSize(style.title_size.at(1), "Y");

  gStyle->SetTitleSize(style.title_offset.at(0), "X");
  gStyle->SetTitleSize(style.title_offset.at(1), "Y");

  gStyle->SetOptStat(0);
};

}