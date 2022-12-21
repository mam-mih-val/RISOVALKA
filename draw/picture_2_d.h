//
// Created by mikhail on 5/14/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_
#define FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_

#include "picture.h"
#include <TH2F.h>

class Picture2D : public Picture, public ReadableObject {
public:
  Picture2D(const std::string &name, const std::array<int, 2> &resolution,
            const std::string &file_name,
            const std::vector<std::string> &objects)
      : Picture(name, resolution), ReadableObject(file_name, objects) {
    histogram_ = ReadObjectFromFile<TH2F>( objects.front() );
  }
  void SetDrawOpt(const std::string &draw_option) {
    draw_option_ = draw_option;
  }
  void
  SetAxisTitles(const std::vector<std::string> &axis_titles) override;

  void SetTitleSize(const std::vector<float> &title_font_size) {
    title_size_ = title_font_size;
  }

  void SetLabelSize(const std::vector<float> &label_size) {
    label_size_ = label_size;
  }

  void SetZRange(const std::array<float, 2> &z_range) { z_range_ = z_range; }
  void Draw() override;
  [[nodiscard]] TH2F *GetHistogram() const { return histogram_; }
  void CopyStyle(Picture2D* other);

protected:
  std::vector<float> title_size_;
  std::vector<float> label_size_;
  TH2F* histogram_{nullptr};
  std::string draw_option_{"colz"};
  std::array<float,2> z_range_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_
