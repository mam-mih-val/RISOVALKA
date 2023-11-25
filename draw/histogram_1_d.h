//protons_qa
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
#define FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_

#include "TH1F.h"
#include "drawable_object.h"
#include "file_manager.h"

class Histogram1D : public DrawableObject {
public:
  Histogram1D() = default;
  Histogram1D(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  Histogram1D(const Histogram1D& other);
  Histogram1D(Histogram1D&& other) = default;
  ~Histogram1D() override;
  void RefreshPoints() override;
  [[nodiscard]] TH1 *GetHistogram() const { return histogram_.get(); }
  void SetHistogram(TH1 *histogram) { histogram_.reset(histogram); }
  friend Histogram1D operator/( const Histogram1D& num, const Histogram1D& den);

protected:
  std::unique_ptr<TH1> histogram_{};
};

#endif // FLOW_DRAWING_TOOLS_SRC_HISTOGRAM_1_D_H_
