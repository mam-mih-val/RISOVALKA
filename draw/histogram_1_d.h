//protons_qa
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
#define FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_

#include "TH1F.h"
#include "drawable_object.h"

class Histogram1D : public DrawableObject {
public:
  Histogram1D() = default;
  Histogram1D(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Histogram1D() override;
  void RefreshPoints() override;
  TH1 *GetHistogram() const { return histogram_; }
  void SetHistogram(TH1F *histogram) { histogram_ = histogram; }
  friend Histogram1D operator/( const Histogram1D& num, const Histogram1D& den);

protected:
  TH1* histogram_;
  ClassDefOverride(Histogram1D, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
