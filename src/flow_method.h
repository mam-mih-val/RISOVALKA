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
  void SetName(const std::string &name) { name_ = name; }
  virtual void ComputeFlow(){
//    auto corr = un_qn_->Projection({"Centrality","mdc_vtx_tracks_rapidity"});
    auto corr = *un_qn_;
    flow_ = new Qn::DataContainerStats ( corr / *(resolution_) );
  }
  virtual void ComputeResolution(){}
  void Write(TFile* file){
    file->cd();
    std::string save_name = "flow_"+name_;
    flow_->Write(save_name.c_str());
    save_name = "resolution_"+name_;
    resolution_->Write(save_name.c_str());
  }
protected:
  std::string name_;
  Qn::DataContainer<Qn::Stats> *un_qn_;
  std::vector<Qn::DataContainer<Qn::Stats>*> qn_qn_;

  Qn::DataContainer<Qn::Stats> *flow_{nullptr};
  Qn::DataContainer<Qn::Stats> *resolution_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_FLOW_METHOD_H_
