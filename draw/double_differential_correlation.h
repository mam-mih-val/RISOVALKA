//
// Created by mikhail on 5/7/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_

#include <TGraphErrors.h>
#include <TF1.h>
#include <DataContainer.hpp>

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"

class DoubleDifferentialCorrelation : public ReadableObject {
public:
  DoubleDifferentialCorrelation() = default;
  DoubleDifferentialCorrelation(const std::string &file_name,
                                const std::vector<std::string> &objects)
      : ReadableObject(file_name, objects) {
    std::vector<Qn::DataContainerStatCalculate> containers;
    for( const auto& name : objects ){
      try {
        containers.emplace_back(
            *(this->ReadObjectFromFile<Qn::DataContainerStatCalculate>(name)));
      } catch (std::exception&) {
        containers.emplace_back(
            *(this->ReadObjectFromFile<Qn::DataContainerStatCollect>(name)));
      }
    }
    correlation_ = containers.front();
    for( size_t i=1; i< containers.size(); ++i )
      correlation_ = correlation_+ containers.at(i);
    correlation_ = correlation_/ (double)containers.size();
  }
  ~DoubleDifferentialCorrelation() override = default;
  Qn::DataContainerStatCalculate &GetCorrelation() {
    return correlation_;
  }
  void Scale( double scale ){ correlation_ = correlation_*scale; }
  void Rebin( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Rebin(axis);
  }
  void Select( const std::vector<Qn::AxisD>& axes){
    for( const auto& axis : axes)
      correlation_ = correlation_.Select(axis);
  }
  void SetSliceVariable(const std::string &name, const std::string &units ){
    slice_variable_name_ = name;
    slice_variable_units_ = units;
  }
  void Project(std::vector<std::string> axes){
    correlation_ = correlation_.Projection(std::move(axes));
  }
  void SetProjectionAxis(const Qn::AxisD &projection_axis) {
    projection_axis_ = projection_axis;
  }
  [[nodiscard]] const std::vector<Graph *> &GetProjections() const { return projections_; }
  void SetSliceAxis(const Qn::AxisD &slice_axis) { slice_axis_ = slice_axis; }
  void SetMarker(int marker) { marker_ = marker; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  void Calculate();
  void SaveToFile( const std::string& file_name );
  void SetErrorOption(const std::string &error_option);
protected:
  void FillGraphs();
  Qn::DataContainerStatCalculate correlation_;
  Qn::AxisD projection_axis_;
  Qn::AxisD slice_axis_;
  std::vector<TGraphErrors*> projection_points_;
  std::string slice_variable_name_;
  std::string slice_variable_units_;
  std::string error_option_;
  std::vector<Graph*> projections_;
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

#endif // FLOW_DRAWING_TOOLS_DRAW_DOUBLE_DIFFERENTIAL_CORRELATION_H_
