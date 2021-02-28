//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_

#include <DataContainer.hpp>
#include <utility>
#include "drawable_object.h"

class Correlation : public DrawableObject {
public:
  Correlation(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Correlation() override;
  TGraphErrors *GetPoints() override;
  Qn::DataContainerStatCalculate &GetCorrelation() {
    return correlation_;
  }
  void Rebin( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Rebin(axis);
  }
  void Select( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Select(axis);
  }
  void Project(std::vector<std::string> axes){
    correlation_ = correlation_.Projection(std::move(axes));
  }

protected:
  Qn::DataContainerStatCalculate correlation_;
  ClassDefOverride(Correlation, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
