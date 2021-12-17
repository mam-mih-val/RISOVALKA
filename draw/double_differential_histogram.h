//
// Created by mikhail on 11/9/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_HISTOGRAM_H_
#define FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_HISTOGRAM_H_

#include <sstream>
#include <iomanip>

#include <TH2.h>

#include "graph.h"
#include "histogram_1_d.h"
#include "readable_object.h"

class DoubleDifferentialHistogram : public ReadableObject {
public:
  DoubleDifferentialHistogram() = default;
  DoubleDifferentialHistogram(const std::string &file_name,
                                const std::vector<std::string> &objects)
      : ReadableObject(file_name, objects) {
    file_name_ = file_name;
    std::vector<TH2*> histograms;
    histograms.reserve(objects.size());
    for( const auto& name : objects ){
      histograms.push_back( this->ReadObjectFromFile<TH2>(name) );
    }
    histo2d_ = dynamic_cast<TH2*>(histograms.front()->Clone());
    for (size_t i=1; i<histograms.size(); ++i) {
      histo2d_->Add( histograms.at(i) );
    }
    histo2d_->Scale( 1.0 / (double) histograms.size() );
  }
  ~DoubleDifferentialHistogram() override = default;
  void Projection( const std::string& axis, int first_bin, int last_bin );
  void SetSliceVariable(const std::string &name, const std::string &units ){
    slice_variable_name_ = name;
    slice_variable_units_ = units;
  }
  void SetMarker(int marker) { marker_ = marker; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  [[nodiscard]] const std::vector<Histogram1D *> &GetProjections() const { return projections_; }
private:
  void ColorObjects();
  TH2* histo2d_{nullptr};
  std::vector<Histogram1D*> projections_;
  std::string slice_variable_name_;
  std::string slice_variable_units_;
  int marker_{kFullCircle};
  std::vector<int> palette_{
      kPink,
      kMagenta+1,
      kViolet-8,
      kBlue,
      kAzure-4,
      kCyan+1,
      kGreen+2,
      kSpring-4,
      kYellow-3,
      kOrange-3,
      kRed,
  };
};

#endif // FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_HISTOGRAM_H_
