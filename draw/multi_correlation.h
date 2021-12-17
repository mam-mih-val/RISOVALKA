//
// Created by mikhail on 8/11/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"
#include <QnTools/DataContainer.hpp>

class MultiCorrelation : public TObject {
public:
  MultiCorrelation() = default;
  MultiCorrelation(MultiCorrelation const &other) :marker_(other.marker_),
                                                    palette_(other.palette_){
      for( auto corr : other.correlations_ )
        correlations_.push_back( new Correlation(*corr) );
  };
  ~MultiCorrelation() override = default;
  void AddCorrelation( const std::string& file, const std::vector<std::string>& objects, const std::string& title );
  void Scale( double scale );
  void Rebin( const std::vector<Qn::AxisD>& axes);
  void Select( const std::vector<Qn::AxisD>& axes);
  void Project(const std::vector<std::string>& axes);
  void SetMarkers(const std::vector<int> &markers) { markers_ = markers; }
  [[nodiscard]] const std::vector<Correlation*> &GetCorrelations() {
    this->ApplyStyle();
    return correlations_;
  }
  void SetMarker(int marker) { marker_ = marker; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  void SetErrorOption(const std::string &error_option);

protected:
  void ApplyStyle();
  std::vector<Correlation*> correlations_;
  std::vector<int> markers_;
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
  ClassDefOverride( MultiCorrelation, 1 );
};

#endif // FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
