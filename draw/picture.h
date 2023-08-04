//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
#define FLOW_DRAWING_TOOLS_SRC_PICTURE_H_

#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TObject.h>
#include <TLegend.h>
#include <TLine.h>

#include "drawable_object.h"
#include <TLatex.h>
#include <utility>

class Picture {
public:
  Picture() = default;
  virtual ~Picture() = default;
  Picture(std::string name, const std::array<int, 2> &resolution);
  [[nodiscard]] TCanvas *GetCanvas() const { return canvas_.get(); }
  virtual void SetAxisTitles(const std::vector<std::string> &axis_titles) {}
  void SetXRange(const std::array<float, 2> &x_range) { x_range_ = x_range; }
  void SetYRange(const std::array<float, 2> &y_range) { y_range_ = y_range; }
  bool IsDrawn() const { return is_drawn_; }
  virtual void Draw() { is_drawn_ = true; };
  void Save( const std::string& type );
  void Save( const std::string& name, const std::string& type );
  void SavePoints(const std::string& name, const std::string& type);
  [[nodiscard]] TF1 *GetZeroLine() const { return zero_line_.get(); }
  void DrawZeroLine(bool draw_zero_line) { draw_zero_line_ = draw_zero_line; }
  void SetAutoLegend(bool auto_legend) { auto_legend_ = auto_legend; }
  void AddText( const TLatex& text, float size=0.04 ){ texts_.push_back ( std::make_unique<TLatex>(text) ); text_sizes_.push_back(size); }
  void AddLine( const TLine& line, int color=kRed, int style=1 ){
    lines_.push_back( std::make_unique<TLine>(line) );
    lines_.back()->SetLineColor(color);
    lines_.back()->SetLineStyle(style);
  }
  void AddLegend( TLegend* legend ){ legends_.push_back( std::unique_ptr<TLegend>(legend) ); auto_legend_ = false; }
  void AddFunction( TF1* function ){ functions_.push_back( std::unique_ptr<TF1>(function) ); }
  void SetLogY(bool is_log_y=true) { is_log_y_ = is_log_y; }
  void SetLogX(bool is_log_x=true) { is_log_x_ = is_log_x; }
  void SetLogZ(bool is_log_z=true) { is_log_z_ = is_log_z; }
  void CopyStyle(Picture* other);

protected:
  std::string name_;
  std::array<int, 2> resolution_;
  std::unique_ptr<TCanvas> canvas_;
  std::unique_ptr<TMultiGraph> stack_;
  std::vector< std::unique_ptr<TF1> > functions_;
  std::vector<std::string> axis_titles_;
  std::unique_ptr<TF1> zero_line_{};
  std::array<float, 2> x_range_;
  std::array<float, 2> y_range_;
  std::vector<std::unique_ptr<TLatex>> texts_;
  std::vector<float> text_sizes_;
  std::vector< std::unique_ptr<TLegend> > legends_;
  std::vector<std::unique_ptr<TLine>> lines_;

  bool draw_zero_line_{true};
  bool is_log_y_{false};
  bool is_log_x_{false};
  bool is_log_z_{false};
  bool is_drawn_{false};
  bool auto_legend_{true};
};

#endif // FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
