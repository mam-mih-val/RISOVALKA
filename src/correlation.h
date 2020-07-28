//
// Created by mikhail on 7/17/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
#define FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_

#include "file_manager.h"
#include <Axis.h>
#include <DataContainer.h>
#include <string>
#include <utility>
#include <vector>

class Correlation {
public:
  explicit Correlation(std::string title) : title_(std::move(title)) {}
  Correlation(std::string title, std::string name, std::vector<std::string> components,
              std::vector<Qn::Axis<double>> rebin_axis,
              std::string projection_axis, double scale_coefficient)
      : title_(std::move(title)), name_(std::move(name)),
        components_(std::move(components)),
        rebin_axis_(std::move(rebin_axis)),
        projection_axis_(std::move(projection_axis)),
        scale_coefficient(scale_coefficient) {}
  virtual ~Correlation() = default;
  [[nodiscard]] const std::string &GetTitle() const { return title_; }
  void SetName(const std::string &name) { name_ = name; }
  void SetRebinAxis(const std::vector<Qn::Axis<double>> &rebin_axis) {
    rebin_axis_ = rebin_axis;
  }
  void SetProjectionAxis(const std::string &projection_axis) {
    projection_axis_ = projection_axis;
  }
  void SetScaleCoefficient(double scale_coefficient) {
    Correlation::scale_coefficient = scale_coefficient;
  }
  void Read();
  [[nodiscard]] Qn::DataContainerStats* GetComponent(const std::string& component){
    return containers_.at(component);
  }

private:
  std::string title_;
  std::string name_;
  std::vector<std::string> components_;
  std::vector<Qn::Axis<double>> rebin_axis_;
  std::string projection_axis_;
  double scale_coefficient{1.0};

  std::map<std::string, Qn::DataContainerStats*> containers_;
};

#endif // FLOW_DRAWING_TOOLS_SRC_CORRELATION_H_
