//
// Created by mikhail on 7/17/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_STACK_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_STACK_H_

#include "config_reader.h"
#include "correlation.h"
class CorrelationStack {
public:
  CorrelationStack() = default;
  virtual ~CorrelationStack() {}
  Qn::DataContainerStats* GetCorrelation(const std::string &name,
                                          const std::string &component){
    for( auto corr : correlations_ )
      if( corr.GetTitle() == name )
        return corr.GetComponent(component);
    std::cout << "CorrelationStack::CorrelationStack: no container with name " << name << std::endl;
    abort();
  }
  std::vector<Qn::DataContainerStats*> GetVectorOfCorrelations(const std::vector<std::string> &names,
                                                               const std::string &component){
    std::vector<Qn::DataContainerStats*> result;
    result.reserve(names.size());
    for( const auto& name : names )
      result.push_back( GetCorrelation(name, component) );
    return result;
  }
  void SetConfig(const std::string& config){ config_reader_.Open(config); }
  void FillStack(){
    correlations_ = config_reader_.Read();
    for( auto& corr : correlations_)
      corr.Read();
  }
private:
  ConfigReader config_reader_;
  std::vector<Correlation> correlations_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_STACK_H_
