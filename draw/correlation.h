//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_

#include "drawable_object.h"
#include <DataContainer.hpp>
#include <utility>

class Correlation : public DrawableObject {
public:
  Correlation() = default;
  Correlation(Correlation const&) = default;
  Correlation(const std::string &file_name,
              const std::vector<std::string> &objects,
              const std::string &title);
  ~Correlation() override;
  void RefreshPoints() override {
    correlation_.SetErrors(Qn::StatCalculate::ErrorType::BOOTSTRAP);
    points_ = Qn::ToTGraph( correlation_ );
    for( int i=0; i<points_->GetN(); ++i ){
      auto y_err = points_->GetErrorY(i);
      if( isnan( y_err ) )
        points_->SetPointError(i, 0., 0.);
    }
    if( fit_ )
      points_->Fit(fit_);
    this->SetMarkerStyle();
    systematical_errors_ = Qn::ToTGraph( correlation_ );
    if( sys_error_value_ > std::numeric_limits<float>::min() ) {
      for (int i = 0; i < systematical_errors_->GetN(); ++i) {
        auto y = systematical_errors_->GetPointY(i);
        auto y_err = systematical_errors_->GetErrorY(i);
        y_err = y * sys_error_value_;
        systematical_errors_->SetPointError(i, 0, y_err);
      }
    }
    systematical_errors_->SetLineColor( color_ );
    systematical_errors_->SetFillColor( color_ );

  }
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
  void Project(std::vector<std::string> axes){
    correlation_ = correlation_.Projection(std::move(axes));
  }
  void Scale(double num){ correlation_ = correlation_*num; }
  friend Correlation operator/( const Correlation& num, const Correlation& den);
  void SetSysErrorValue(float sys_error_value) {
    sys_error_value_ = sys_error_value;
  }
  float GetSysErrorValue() const { return sys_error_value_; }
  TGraphErrors *GetSystematicalErrors() const { return systematical_errors_; }

protected:
  Qn::DataContainerStatCalculate correlation_;
  float sys_error_value_{0.0};
  TGraphErrors* systematical_errors_{nullptr};
  ClassDefOverride(Correlation, 1)
};


#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
