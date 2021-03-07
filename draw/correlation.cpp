//
// Created by mikhail on 2/28/21.
//

#include "correlation.h"
ClassImp(Correlation);

Correlation::Correlation(const std::string &file_name,
                         const std::vector<std::string> &objects,
                         const std::string &title)
    : DrawableObject(file_name, objects, title) {
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
Correlation::~Correlation() {}

Correlation operator/( const Correlation& num, const Correlation& den){
  Correlation result;
  result.title_ = num.title_+"_ratio";
  result.marker_ = num.marker_;
  result.color_ = num.color_;
  auto num_container = num.correlation_;
  auto den_container = den.correlation_;
  auto res_container = num_container / den_container;
  result.correlation_ = res_container;

  return result;
}