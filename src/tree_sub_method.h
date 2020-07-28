//
// Created by mikhail on 7/17/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_TREE_SUB_METHOD_H_
#define FLOW_DRAWING_TOOLS_SRC_TREE_SUB_METHOD_H_
#include "flow_method.h"

class TreeSubMethod : public FlowMethod {
public:
  TreeSubMethod() = default;
  ~TreeSubMethod() = default;
  void ComputeFlow() override { FlowMethod::ComputeFlow(); }
  void ComputeResolution() override {
    auto corr1 = *qn_qn_.at(0);
    auto corr2 = *qn_qn_.at(1);
    auto corr3 = *qn_qn_.at(2);
    resolution_ = new Qn::DataContainerStats(Sqrt( corr1*corr2 / corr3 ) );
  }
};

#endif // FLOW_DRAWING_TOOLS_SRC_TREE_SUB_METHOD_H_
