//
// Created by mikhail on 7/17/20.
//

#include "correlation.h"

void Correlation::Read(){
  Qn::DataContainerStats* ptr_obj;
  for( const auto& component : components_ ) {
    std::string name = name_ + "_" + component;
    ptr_obj = FileManager::Instance()->GetObject<Qn::DataContainerStats>(name);
    auto obj = *ptr_obj;
    for( const auto& axis : rebin_axis_)
      obj = obj.Rebin(axis);
    if( !projection_axis_.empty() )
      obj = obj.Projection({projection_axis_});
    obj = obj*scale_coefficient;
    containers_.insert({component, new Qn::DataContainerStats(obj)});
  }
}