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

#include "drawable_object.h"
#include <TLatex.h>
#include <utility>

class Picture {
public:
  Picture() = default;
  virtual ~Picture() = default;
  Picture(std::string name, const std::array<int, 2> &resolution);
  [[nodiscard]] TCanvas *GetCanvas() const { return canvas_; }
  virtual void SetAxisTitles(const std::vector<std::string> &axis_titles) {}
  void SetXRange(const std::array<float, 2> &x_range) { x_range_ = x_range; }
  void SetYRange(const std::array<float, 2> &y_range) { y_range_ = y_range; }
  bool IsDrawn() const { return is_drawn_; }
  virtual void Draw() { is_drawn_ = true; };
  void Save( const std::string& type );
  void Save( const std::string& name, const std::string& type );
  void SavePoints(const std::string& name, const std::string& type);
  [[nodiscard]] TF1 *GetZeroLine() const { return zero_line_; }
  void DrawZeroLine(bool draw_zero_line) { draw_zero_line_ = draw_zero_line; }
  void SetAutoLegend(bool auto_legend) { auto_legend_ = auto_legend; }
  void AddText( const TLatex& text, float size=0.04 ){ texts_.push_back(new TLatex(text)); text_sizes_.push_back(size); }
  void AddLegend( TLegend* legend ){ legends_.push_back( legend ); auto_legend_=false; }
  void AddFunction( TF1* function ){ functions_.push_back(function); }
  void SetLogY(bool is_log_y=true) { is_log_y_ = is_log_y; }
  void SetLogX(bool is_log_x=true) { is_log_x_ = is_log_x; }
  void SetLogZ(bool is_log_z=true) { is_log_z_ = is_log_z; }
  void CopyStyle(Picture* other);

protected:
  std::string name_;
  std::array<int, 2> resolution_;
  TCanvas* canvas_;
  TMultiGraph* stack_;
  std::vector<TF1*> functions_;
  std::vector<std::string> axis_titles_;
  TF1* zero_line_{nullptr};
  bool draw_zero_line_{true};
  std::array<float, 2> x_range_;
  std::array<float, 2> y_range_;
  std::vector<TLatex*> texts_;
  std::vector<float> text_sizes_;
  std::vector<TLegend*> legends_;
  bool is_log_y_{false};
  bool is_log_x_{false};
  bool is_log_z_{false};
  bool is_drawn_{false};
  bool auto_legend_{true};
};

#endif // FLOW_DRAWING_TOOLS_SRC_PICTURE_H_
