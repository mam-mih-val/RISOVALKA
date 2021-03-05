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
#include <TProfile.h>
#include <TH2F.h>
#include <TLegend.h>

#include <Axis.hpp>
#include <DataContainer.hpp>

#include "file_manager.h"

namespace Draw{

struct PictureConfig {
  std::string save_name;
  std::vector<std::string> axes_titles;
  std::vector<std::string> texts;
  std::string ratio_reference_title;
  std::array<short, 2> resolution;
  std::vector<double> x_axis_range;
  std::vector<double> y_axis_range;
  std::vector<double> z_axis_range;
  std::vector<double> ratio_range;
  std::vector<std::vector<double>> text_positions;
  std::vector<double> text_sizes;
  std::vector<double> legend_position;
  std::vector<std::string> formulas;
  int reference_marker;
  int reference_color;
  bool is_reference_line;
  bool save_points=false;
};

struct StyleConfig{
  double pad_left_margin;
  double pad_right_margin;
  double pad_bottom_margin;
  double legend_border_size;
  double frame_line_width;
  std::array<double, 3> title_size;
  std::array<double, 3> title_offset;
  std::array<double, 3> label_offset;
  std::array<double, 3> label_size;
  int marker_size;
  int line_width;
};

struct CorrelationConfig {
  std::string file;
  std::vector<std::string> names;
  std::string title;
  std::vector<Qn::AxisD> selection_axes;
  std::vector<Qn::AxisD> rebin_axes;
  std::string projection_axis;
  double scale{1.0};
  bool is_line{false};
  bool is_in_legend{true};
  int color;
  int marker;
};
struct GraphConfig {
  std::string file;
  std::string name;
  std::string title;
  int color;
  int marker;
};
struct Histogram1DConfig {
  std::string file;
  std::string name;
  std::string title;
  double scale;
  int color;
  int marker;
};
struct Histogram2DConfig {
  std::string file;
  std::string name;
  bool is_log_z;
  bool is_unite;
};

void DrawHistogram2D( const PictureConfig &picture_config, const Histogram2DConfig & histo );

void DrawHistograms1D( const PictureConfig &picture_config,
                      const std::vector<Histogram1DConfig>&histogram_configs,
                      const std::vector<Histogram1DConfig>&profile_configs );

void Draw1D( const PictureConfig &picture_config, const std::vector<CorrelationConfig>&correlation_configs,
            const std::vector<GraphConfig>&graph_configs, const std::vector<Histogram1DConfig>&histogram_configs);

void CompareCorrelations( const PictureConfig &picture_config, const std::vector<CorrelationConfig>& reference_configs,
                          const std::vector<CorrelationConfig>& compare_configs);

void SetStyle( const StyleConfig & style );
}

#endif // FLOW_DRAWING_TOOLS_SRC_DRAW_FUNCTIONS_H_
