//
// Created by mikhail on 8/20/20.
//
#include "draw_functions.h"
#include <TStyle.h>

namespace Draw{

void DrawHistogram2D( const Picture&picture_config, const Histogram2D& histo ){
  auto canvas = new TCanvas("canv", "", picture_config.resolution.at(0),
                            picture_config.resolution.at(1));
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
    histo2d->Scale( 1./(double) histo2d->GetEntries() );
  histo2d->Draw("colz");
  if( text )
    text->Draw("same");
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
  std::string axes_title;
  try {
    axes_title = ";" + picture_config.axes_titles.at(0) + ";" +
                             picture_config.axes_titles.at(1);
  }catch (const std::exception&) {}
  auto* graph_stack = new TMultiGraph("graphs", axes_title.c_str());
  auto* histo_stack = new THStack( "histo", "" );

  for( const auto& config : correlation_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStats> containers;
    for( const auto& name : config.names ) {
      auto container =
          *(FileManager::GetObject<Qn::DataContainerStats>(name));
      for (const auto &axis : config.rebin_axes)
        container = container.Rebin(axis);
      container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }
    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    containers.at(0).SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
    auto graph = Qn::ToTGraph( containers.at(0) );
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
  if( text )
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
  std::string result_name;
  std::string ratio_name;
  try {
    result_name = ";;" + picture_config.axes_titles.at(1);
    ratio_name = ";"+picture_config.axes_titles.at(0) + "; #frac{"+
        picture_config.axes_titles.at(1)+ "}{"+picture_config.ratio_reference_title+"}";
  }catch (const std::exception&) {}
  auto* result_stack = new TMultiGraph("results", result_name.c_str());
  auto* ratio_stack = new TMultiGraph("ratio", ratio_name.c_str());
  std::vector<Qn::DataContainerStats> references;
  for( const auto& config : reference_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStats> containers;
    for( const auto& name : config.names ) {
      auto container =
          *(FileManager::GetObject<Qn::DataContainerStats>(name));
      for (const auto &axis : config.rebin_axes)
        container = container.Rebin(axis);
      container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }
    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    references.push_back(containers.at(0));
  }
  for( size_t i=1; i<std::size(references); ++i ){
    references.at(0) = references.at(0) + references.at(i);
  }
  references.at(0) = references.at(0) * ( 1.0 / (double) std::size(references) );
  for( const auto& config : compare_configs){
    FileManager::Open(config.file);
    std::vector<Qn::DataContainerStats> containers;
    for( const auto& name : config.names ) {
      auto container =
          *(FileManager::GetObject<Qn::DataContainerStats>(name));
      for (const auto &axis : config.rebin_axes)
        container = container.Rebin(axis);
      container = container.Projection({config.projection_axis});
      container = container * config.scale;
      containers.emplace_back(container);
    }
    for (size_t i = 1; i < std::size(containers); ++i)
      containers.at(0) = containers.at(0) + containers.at(i);
    containers.at(0) = containers.at(0) * (1.0/ (double ) std::size(containers));
    containers.at(0).SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
    auto ratio = containers.at(0) / references.at(0);
    ratio.SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
    auto graph = Qn::ToTGraph( containers.at(0) );
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
  references.at(0).SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
  auto graph = Qn::ToTGraph( references.at(0) );
  graph->SetTitle(picture_config.ratio_reference_title.c_str());
  graph->SetLineColor(kBlack);
  graph->SetMarkerColor(kBlack);
  graph->SetMarkerStyle(kFullCircle);
  result_stack->Add(graph);
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
  try{
    gPad->BuildLegend(picture_config.legend_position.at(0), picture_config.legend_position.at(1),
                      picture_config.legend_position.at(2), picture_config.legend_position.at(3),
                      "", "P");
  } catch (std::out_of_range&) {
    gPad->BuildLegend();
  }
  auto line_zero = new TF1( "zero", "0", -100, 100 );
  line_zero->SetLineColor(kBlue);
  line_zero->Draw("same");
  text->Draw();
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
  line_one->SetLineColor(kRed);
  line_one->Draw("same");
  line_zero->Draw("same");
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
  gStyle->SetTitleSize(style.title_size.at(2), "Z");

  gStyle->SetTitleSize(style.title_offset.at(0), "X");
  gStyle->SetTitleSize(style.title_offset.at(1), "Y");
  gStyle->SetTitleSize(style.title_offset.at(2), "Z");

  gStyle->SetOptStat(0);
};

}