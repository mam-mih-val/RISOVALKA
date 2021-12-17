//
// Created by mikhail on 3/31/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_HISTOGRAM_2_D_H_
#define FLOW_DRAWING_TOOLS_DRAW_HISTOGRAM_2_D_H_

#include "histogram_1_d.h"
#include <TH2F.h>

class Histogram2D : public Histogram1D {
public:
  Histogram2D(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Histogram2D() override;
  void RefreshPoints() override;
  void Projection(const std::string& axis, int first_bin, int last_bin);
  TH2* GetHisto2D() const { return histo2d_; }

protected:
  TH2* histo2d_{nullptr};
  ClassDefOverride(Histogram2D, 1);
};

#endif // FLOW_DRAWING_TOOLS_DRAW_HISTOGRAM_2_D_H_
