//
// Created by mikhail on 7/8/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_RND_NOT_EXT_METHOD_H_
#define FLOW_DRAWING_TOOLS_SRC_RND_NOT_EXT_METHOD_H_

#include "flow_method.h"

class RndNotExtMethod : public FlowMethod {
public:
  RndNotExtMethod() = default;
  ~RndNotExtMethod() override = default;

  void ComputeFlow() override { FlowMethod::ComputeFlow(); }
  void ComputeResolution() override {
    resolution_ = new Qn::DataContainerStats(Sqrt( *qn_qn_.at(0) ) );
  }

};

#endif // FLOW_DRAWING_TOOLS_SRC_RND_NOT_EXT_METHOD_H_
