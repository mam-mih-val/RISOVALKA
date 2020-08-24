//
// Created by mikhail on 8/20/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_DRAW_FUNCTIONS_H_
#define FLOW_DRAWING_TOOLS_SRC_DRAW_FUNCTIONS_H_

#include <string>

#include <TCanvas.h>
#include <TPad.h>
#include <TLatex.h>
#include <TMultiGraph.h>
#include <THStack.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>

#include <Axis.hpp>
#include <DataContainer.hpp>

#include "file_manager.h"

namespace Draw{

struct Picture{
  std::string save_name;
  std::string axis_titles;
  std::string text;
  std::array<short, 2> resolution;
  std::vector<double> x_axis_range;
  std::vector<double> y_axis_range;
  std::vector<double> text_position;
  std::vector<double> legend_position;
};

struct Style{
  double pad_left_margin;
  double pad_right_margin;
  double pad_bottom_margin;
  double legend_border_size;
  double frame_line_width;
  std::array<double, 2> title_size;
  std::array<double, 2> title_offset;
  int marker_size;
  int line_width;
};

struct Correlation{
  std::string file;
  std::string name;
  std::string title;
  std::vector<Qn::AxisD> rebin_axes;
  std::string projection_axis;
  double scale{1.0};
  int color;
  int marker;
};
struct Graph{
  std::string file;
  std::string name;
  std::string title;
  int color;
  int marker;
};
struct Histogram1D{
  std::string file;
  std::string name;
  std::string title;
  int color;
  int marker;
};
struct Histogram2D{
  std::string file;
  std::string name;
};

void DrawHistogram2D( const Picture&picture_config, const Histogram2D& histo );

void Draw1D( const Picture&picture_config, const std::vector<Correlation>&correlation_configs,
            const std::vector<Graph>&graph_configs, const std::vector<Histogram1D>&histogram_configs);

void CompareCorrelations( const Picture&picture_config, const std::vector<Correlation>& reference_configs,
                          const std::vector<Correlation>& compare_configs);

void SetStyle( const Style& style );
}

#endif // FLOW_DRAWING_TOOLS_SRC_DRAW_FUNCTIONS_H_
