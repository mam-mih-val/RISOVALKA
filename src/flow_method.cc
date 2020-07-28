//
// Created by mikhail on 7/8/20.
//

#include "flow_method.h"

void FlowMethod::ComputeFlow(){
  auto num = *un_qn_;
  auto den = *resolution_;
  for( auto& bin : num  ) {
    bin.SetWeights(Qn::Stats::Weights::OBSERVABLE);
    bin.ResetBits(Qn::Stats::CORRELATEDERRORS);
  }
  for( auto& bin : den  ) {
    bin.SetWeights(Qn::Stats::Weights::REFERENCE);
    bin.ResetBits(Qn::Stats::CORRELATEDERRORS);
  }
  flow_ = new Qn::DataContainerStats (num / den );
}

void FlowMethod::ResolutionProjection(){
  Qn::DataContainer<Qn::Stats> result(*resolution_);
  for( const auto& axis : resolution_proj_axis_ )
    result = result.Rebin(axis);
  result = result.Projection({"Centrality"});
  resolution_ = new Qn::DataContainer<Qn::Stats>(result);
}

void FlowMethod::Write(TFile* file){
  file->cd();
  std::string save_name = "flow_"+name_;
  flow_->Write(save_name.c_str());
  save_name = "resolution_"+name_;
  resolution_->Write(save_name.c_str());
}