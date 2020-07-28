//
// Created by mikhail on 7/8/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_FLOW_METHOD_H_
#define FLOW_DRAWING_TOOLS_SRC_FLOW_METHOD_H_

#include <vector>

#include <DataContainer.h>
#include <Stats.h>

class FlowMethod {
public:
  FlowMethod() : un_qn_(), flow_(), resolution_() {};
  virtual ~FlowMethod() = default;
  FlowMethod(FlowMethod& other) = default;

  void SetUnQn(Qn::DataContainer<Qn::Stats>* un_qn) { un_qn_ = un_qn; }
  void SetQnQn(const std::vector<Qn::DataContainer<Qn::Stats>*> &qn_qn) {
    qn_qn_ = qn_qn;
  }
  void SetResolutionProjAxis(
      const std::vector<Qn::Axis<double>> &resolution_proj_axis) {
    resolution_proj_axis_ = resolution_proj_axis;
  }
  void AddResolutionRebinAxis(const Qn::Axis<double>& axis ){
    resolution_proj_axis_.emplace_back(axis);
  }
  void ResolutionProjection();
  void SetName(const std::string &name) { name_ = name; }
  virtual void ComputeFlow();
  virtual void ComputeResolution(){}
  void Write(TFile* file);
protected:
  std::string name_;
  Qn::DataContainer<Qn::Stats> *un_qn_;
  std::vector<Qn::DataContainer<Qn::Stats>*> qn_qn_;
  std::vector<Qn::Axis<double>> resolution_proj_axis_;

  Qn::DataContainer<Qn::Stats> *flow_{nullptr};
  Qn::DataContainer<Qn::Stats> *resolution_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_FLOW_METHOD_H_
