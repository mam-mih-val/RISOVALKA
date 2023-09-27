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
  MultiCorrelation(MultiCorrelation const &other) {
    markers_ = other.markers_;
    palette_ = other.palette_;
    for( const auto& corr : other.correlations_ )
      correlations_.push_back( std::make_unique<Correlation>( *corr ) );
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
  void SetStyle(){ ColorObjects(correlations_); };
  [[nodiscard]] const std::vector<std::unique_ptr<Correlation>> &GetCorrelations() {
    ColorObjects(correlations_);
    return correlations_;
  }
  std::vector<std::unique_ptr<Correlation>>& operator*(){
    return correlations_;
  }
  void SetErrorOption(const std::string &error_option);

protected:
  std::vector<std::unique_ptr<Correlation>> correlations_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_MULTI_CORRELATION_H_
