//
// Created by mikhail on 5/14/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_
#define FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_

#include <TH2F.h>

#include "picture.h"
#include "file_manager.h"

class Picture2D : public Picture {
public:
  Picture2D(const std::string &name, const std::array<int, 2> &resolution,
            const std::string &file_name,
            const std::string &object)
      : Picture(name, resolution) {
    histogram_ = std::unique_ptr<TH2>(FileManager::ReadObject<TH2>( file_name, object ));
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
  [[nodiscard]] TH2 *GetHistogram() const { return histogram_.get(); }
  void CopyStyle(Picture2D* other);

protected:
  std::vector<float> title_size_;
  std::vector<float> label_size_;
  std::unique_ptr<TH2> histogram_{};
  std::string draw_option_{"colz"};
  std::array<float,2> z_range_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_PICTURE_2_D_H_
