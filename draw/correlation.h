//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_

#include <utility>

#include <DataContainer.hpp>

#include "drawable_object.h"

class Correlation : public DrawableObject {
public:
  Correlation() = default;
  Correlation(Correlation const&) = default;
  Correlation(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Correlation() override;
  void RefreshPoints() override;
  Qn::DataContainerStatCalculate &GetCorrelation() {
    return average_;
  }
  void Rebin( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes) {
      average_ = average_.Rebin(axis);
      for( auto& container : combinations_ )
        container = container.Rebin(axis);
    }
  }
  void Select( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes) {
      average_ = average_.Select(axis);
      for( auto& container : combinations_ )
        container = container.Select(axis);
    }
  }
  void Project(const std::vector<std::string>& axes){
    average_ = average_.Projection(axes);
    for( auto& container : combinations_ )
      container = container.Projection(axes);
  }
  void Scale(double num){
    average_ = average_ *num;
    for( auto& container : combinations_ )
      container = container*num;
  }
  friend Correlation operator/( const Correlation& num, const Correlation& den);
  void SetCalculateSystematicsFromVariation(
      bool calculate_systematics_from_variation = true) {
    calculate_systematics_from_variation_ =
        calculate_systematics_from_variation;
  }

protected:
  Qn::DataContainerStatCalculate average_;
  std::vector<Qn::DataContainerStatCalculate> combinations_;
  bool calculate_systematics_from_variation_;
  ClassDefOverride(Correlation, 1)
};


#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
