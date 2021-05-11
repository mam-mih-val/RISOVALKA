//
// Created by mikhail on 5/6/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_DV1DY_CONTAINER_H_
#define FLOW_DRAWING_TOOLS_DRAW_DV1DY_CONTAINER_H_

#include <TGraphErrors.h>
#include <TF1.h>
#include <DataContainer.hpp>

#include "correlation.h"
#include "graph.h"
#include "readable_object.h"

class dv1dy_container : public ReadableObject {
public:
  dv1dy_container() = default;
  dv1dy_container(const std::string &file_name,
                 const std::vector<std::string> &objects)
      : ReadableObject(file_name, objects) {
    std::vector<Qn::DataContainerStatCalculate> conatiners;
    for( const auto& name : objects ){
      try {
        conatiners.emplace_back(
            *(this->ReadObjectFromFile<Qn::DataContainerStatCalculate>(name)));
      } catch (std::exception&) {
        conatiners.emplace_back(
            *(this->ReadObjectFromFile<Qn::DataContainerStatCollect>(name)));
      }
    }
    correlation_ = conatiners.front();
    for( size_t i=1; i<conatiners.size(); ++i )
      correlation_ = correlation_+conatiners.at(i);
    correlation_ = correlation_/ (double) conatiners.size();
  }
  ~dv1dy_container() override = default;
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
  void SetSliceVariable(const std::string &name, const std::string &units ){
    slice_variable_name_ = name;
    slice_variable_units_ = units;
  }
  void Project(std::vector<std::string> axes){
    correlation_ = correlation_.Projection(std::move(axes));
  }
  void SetFormula(const std::string &formula) {
    dv1dy_container::formula_ = formula;
  }
  void SetSlopeMarker(int marker) { slope_marker_ = marker; }
  void SetOffsetMarker(int marker) { offset_marker_ = marker; }
  const std::vector<Graph *> &GetSlopes() const { return slopes_; }
  const std::vector<Graph *> &GetOffsets() const { return offsets_; }
  const std::vector<TGraphErrors *> &GetSlopeGraphs() const { return slope_graphs_; }
  const std::vector<TGraphErrors *> &GetOffsetGraphs() const {return offset_graphs_;}
  void Calculate( const Qn::AxisD& remaining_axis, const Qn::AxisD& slice_axis, const Qn::AxisD& rapidity_axis );
  void SaveToFile( const std::string& file_name );
  void SetPalette(const std::vector<int> &palette);

protected:
  void FillGraphs();
  std::string formula_{"pol1"};
  Qn::DataContainerStatCalculate correlation_;
  std::vector<TGraphErrors*> projections_;
  std::vector<TGraphErrors*> slope_graphs_;
  std::vector<TGraphErrors*> offset_graphs_;
  std::string slice_variable_name_;
  std::string slice_variable_units_;
  std::vector<Graph*> slopes_;
  std::vector<Graph*> offsets_;
  int slope_marker_{kFullCircle};
  int offset_marker_{kOpenCircle};
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

#endif // FLOW_DRAWING_TOOLS_DRAW_DV1DY_CONTAINER_H_
