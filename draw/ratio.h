//
// Created by mikhail on 3/6/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_RATIO_H_
#define FLOW_DRAWING_TOOLS_DRAW_RATIO_H_

#include "picture.h"
#include <TStyle.h>
#include <TH1F.h>

template <class T>
class Ratio : public Picture {
public:
  Ratio() = default;
  ~Ratio() override {}
  Ratio(const std::string &name, const std::array<int, 2> &resolution)
      : Picture(name, resolution) {
    std::string stack_name = name_+"_ratio_stack";
    ratio_stack_ = std::make_unique<TMultiGraph>(stack_name.c_str(), "");
  }
  void SetObjects( std::vector<std::unique_ptr<T>>&& vec ){ objects_ = std::move(vec); }
  void AddObject( T* obj ){ objects_.emplace_back(obj); }
  void AddObject( std::unique_ptr<T>&& obj ){ objects_.emplace_back( std::move(obj.get()) ); }

  void AddDrawableObject( DrawableObject* obj ){ drawable_objects_.emplace_back(obj); }

  void SetRatioRange(const std::array<float, 2> &ratio_range) {
    ratio_range_ = ratio_range;
  }
  void SetReference(T *reference) { reference_ = std::unique_ptr<T>(reference); }
  void SetReference(std::unique_ptr<T>&& reference) { reference_ = std::move(reference); }

  void Draw() override{
    this->Picture::Draw();
//    if(auto_legend_){
//      assert(legends_.empty());
//      legends_.emplace_back( new TLegend() );
//    }
    assert(reference_);
    for( const auto& obj : objects_ ){
      if( obj->IsLine() ) {
        stack_->Add( obj->ReleasePoints(), "L");
      } else {
        stack_->Add(obj->ReleasePoints(), "P");
      }
      ratios_.emplace_back( new T( *obj / *reference_ ) );
    }
    if( reference_->IsLine() ) {
      stack_->Add(reference_->ReleasePoints(), "L");
    } else {
      stack_->Add(reference_->ReleasePoints(), "P");
    }
    for( const auto& obj : ratios_ ){
      if( obj->IsLine() ) {
        ratio_stack_->Add(obj->ReleasePoints(), "L");
      } else {
        ratio_stack_->Add(obj->ReleasePoints(), "P");
      }
    }
    for( auto obj : drawable_objects_ ){
      if( obj->IsLine() ) {
        std::string opt{"L+" + obj->GetErrorOption()};
        stack_->Add(obj->ReleasePoints(), opt.c_str());
      } else {
        std::string opt{"P+" + obj->GetErrorOption()};
        stack_->Add(obj->ReleasePoints(), opt.c_str());
      }
    }
    auto result_pad = new TPad("result", "result", 0.0, 0.35, 1.0, 1.0);
    auto ratio_pad = new TPad("ratio", "ratio", 0.0, 0.0, 1.0, .35);
    auto ratio_pad_scale = 1.0/0.35-0.3;
    auto result_pad_scale = 1.0/0.75;
    result_pad->cd();
    result_pad->SetBottomMargin(0);
    stack_->Draw("AP");
    if( x_range_.at(0) < x_range_.at(1) ) {
      stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
      stack_->Draw();
    }
    if( y_range_.at(0) < y_range_.at(1) ) {
      stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
      stack_->Draw();
    }
    stack_->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("Y")*result_pad_scale, "Y");
    stack_->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("Y")*result_pad_scale, "Y");
    stack_->GetHistogram()->SetTitleOffset(gStyle->GetTitleOffset("Y")*result_pad_scale, "Y");
    if(draw_zero_line_)
      zero_line_->Draw("same");
    int i=0;
    for( const auto& text : texts_ ){
      text->SetNDC();
      text->SetTextSize(text_sizes_.at(i));
      text->Draw("same");
      ++i;
    }
    for( const auto& legend : legends_) {
      assert(legend);
      legend->Draw("same");
    }
    ratio_pad->cd();
    ratio_pad->SetTopMargin(0);
    ratio_pad->SetBottomMargin(gStyle->GetPadBottomMargin()*ratio_pad_scale);
    ratio_stack_->Draw("AP");
    if( x_range_.at(0) < x_range_.at(1) ) {
      ratio_stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
      ratio_stack_->Draw();
    }
    if( y_range_.at(0) < y_range_.at(1) ) {
      ratio_stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
      ratio_stack_->Draw();
    }
    if( ratio_range_.at(0) != ratio_range_.at(1) ) {
      ratio_stack_->GetYaxis()->SetRangeUser(ratio_range_.at(0),
                                             ratio_range_.at(1));
      ratio_stack_->Draw();
    }
    ratio_stack_->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("Y")* ratio_pad_scale, "Y");
    ratio_stack_->GetHistogram()->SetLabelSize(gStyle->GetLabelSize("X")* ratio_pad_scale, "X");
    ratio_stack_->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("Y")* ratio_pad_scale, "Y");
    ratio_stack_->GetHistogram()->SetTitleOffset(gStyle->GetTitleOffset("Y")* ratio_pad_scale, "Y");
    ratio_stack_->GetHistogram()->SetTitleSize(gStyle->GetTitleSize("X")* ratio_pad_scale, "X");
    ratio_stack_->GetYaxis()->SetNdivisions(5,5, 0);
    if( std::size( axis_titles_ ) > 0 )
      ratio_stack_->GetXaxis()->SetTitle( axis_titles_.at(0).c_str() );
    if( std::size( axis_titles_ ) > 2 )
      ratio_stack_->GetYaxis()->SetTitle( axis_titles_.at(2).c_str() );

    auto line_one = new TF1( "one", "1", -100, 100 );
    line_one->SetLineColor( reference_->GetColor() );
    line_one->Draw("same");
    canvas_->cd();
    result_pad->Draw();
    ratio_pad->Draw();
    canvas_->Draw();
  }
  void SetAxisTitles(const std::vector<std::string> &axis_titles) override{
    assert(axis_titles.size()==3);
    axis_titles_ = axis_titles;
    auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1);
    stack_->SetTitle( title.c_str() );
  };
  void CopyStyle(Ratio<T>* other){
    this->Picture::CopyStyle(other);
    ratio_range_ = other->ratio_range_;
  }


protected:
  std::unique_ptr<T> reference_{};
  std::vector< std::unique_ptr<T> > objects_{};
  std::vector< std::unique_ptr<T> > ratios_{};
  std::array<float, 2> ratio_range_{};
  std::unique_ptr<TMultiGraph> ratio_stack_{};
  std::vector<DrawableObject*> drawable_objects_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_RATIO_H_
