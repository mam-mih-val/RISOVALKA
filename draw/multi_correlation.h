//
// Created by mikhail on 8/11/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"
#include "palette.h"
#include <DataContainer.hpp>

class MultiCorrelation : public Palette {
public:
  MultiCorrelation() = default;
  MultiCorrelation(MultiCorrelation const &other) :markers_(other.markers_),
                                                    palette_(other.palette_){
      for( auto corr : other.correlations_ )
        correlations_.push_back( new Correlation(*corr) );
  };
  ~MultiCorrelation() override = default;
  void AddCorrelation( const std::string& file,
                       const std::vector<std::string>& objects,
                       const std::string& title,
                       const std::vector<double>& weights_={});
  void Scale( double scale );
  void Rebin( const std::vector<Qn::AxisD>& axes);
  void Select( const std::vector<Qn::AxisD>& axes);
  void Project(const std::vector<std::string>& axes);
  [[nodiscard]] const std::vector<Correlation*> &GetCorrelations() {
    this->ColorObjects(correlations_);
    return correlations_;
  }
  void SetErrorOption(const std::string &error_option);

protected:
  std::vector<Correlation*> correlations_;
  std::vector<int> markers_{kFullCircle};
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

#endif // FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
