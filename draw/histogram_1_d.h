//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
#define FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_

#include "drawable_object.h"
#include "TH1F.h"

class Histogram1D : public DrawableObject {
public:
  Histogram1D(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Histogram1D() override;
  virtual TGraphErrors *GetPoints() override;
  TH1F *GetHistogram() const { return histogram_; }
  void SetHistogram(TH1F *histogram) { histogram_ = histogram; }

protected:
  TH1F* histogram_;
  ClassDefOverride(Histogram1D, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
