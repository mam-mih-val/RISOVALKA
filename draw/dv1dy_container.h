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
  void Scale( double scale ){
    correlation_ = correlation_*scale;
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
  void SetMarkers(const std::vector<int> &markers) { markers_ = markers; }
  const std::vector<Graph *> GetParGraphs(int i){ return fit_par_graphs_.at(i); }
  const std::vector<Graph *> &GetSlopes() const { return fit_par_graphs_.at(1); }
  const std::vector<Graph *> &GetOffsets() const { return fit_par_graphs_.at(0); }
  const std::vector<TGraphErrors *> &GetParPoints(int i) const { return fit_par_points_.at(i); }
  const std::vector<TGraphErrors *> &GetSlopePoints() const { return fit_par_points_.at(1); }
  const std::vector<TGraphErrors *> &GetOffsetPoints() const {return fit_par_points_.at(1);}
  void Calculate( const Qn::AxisD& remaining_axis, const Qn::AxisD& slice_axis, const Qn::AxisD& rapidity_axis );
  void SaveToFile( const std::string& file_name );
  void SetPalette(const std::vector<int> &palette);
  void SetFixedParameters(const std::map<int, double> &fixed_parameters) {
    fixed_parameters_ = fixed_parameters;
  }
  void IsTrueOffset( bool is=true ){ is_true_offset_=is; }

protected:
  void FillGraphs();
  std::string formula_{"pol1"};
  bool is_true_offset_{false}; // Works only with polynomial fits
  std::map<int, double> fixed_parameters_;
  Qn::DataContainerStatCalculate correlation_;
  std::vector<TGraphErrors*> projections_;
  std::vector<std::vector<TGraphErrors*>> fit_par_points_;
  std::vector<std::vector<Graph*>> fit_par_graphs_;
  std::string slice_variable_name_;
  std::string slice_variable_units_;
  std::vector<int> markers_;
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
