//
// Created by mikhail on 3/6/21.
//

#include "heap_picture.h"
#include "TH1.h"

void HeapPicture::AddDrawable( DrawableObject* obj ){
  assert(obj);
  if(auto_legend_ && legends_.empty()){
    legends_.emplace_back( new TLegend() );
  }
  auto points = obj->ReleasePoints();
  if( obj->IsLine() ){
    std::string opt{"L+" + obj->GetErrorOption()};
    stack_->Add(points, opt.c_str());
    if( auto_legend_ )
      legends_.back()->AddEntry(points, obj->GetTitle().c_str(),"L");
    if( obj->GetSysErrorPoints() )
      stack_->Add( obj->GetSysErrorPoints(), "L+2" );
  } else{
    std::string opt{"P+" + obj->GetErrorOption()};
    stack_->Add(points, opt.c_str());
    if( auto_legend_ )
      legends_.back()->AddEntry(points, obj->GetTitle().c_str(),"P");
    if( obj->GetSysErrorPoints() )
      stack_->Add( obj->GetSysErrorPoints(), "P+2" );
  }
}

void HeapPicture::AddDrawable( const std::unique_ptr<DrawableObject>& obj ) {
  assert(obj);
  AddDrawable(obj.get());
}
void HeapPicture::AddDrawables( const std::vector<DrawableObject*>& objects ){
  for( auto obj : objects ){
    this->AddDrawable(obj);
  }
}

void HeapPicture::Draw() {
  this->Picture::Draw();
  canvas_->cd();
  if(is_log_x_)
    gPad->SetLogx();
  if(is_log_y_)
    gPad->SetLogy();
  stack_->Draw("APL");
  if( x_range_.at(0) < x_range_.at(1) ) {
    stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
    zero_line_->SetRange(x_range_.at(0), x_range_.at(1));
    stack_->Draw();
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
    stack_->Draw();
  }
  if(draw_zero_line_)
    zero_line_->Draw("same");
  int i=0;
  for( const auto& text : texts_ ){
    text->SetNDC();
    text->SetTextSize(text_sizes_.at(i));
    text->Draw("same+batch");
    ++i;
  }
  for(const auto& legend : legends_) {
    assert(legend);
    legend->SetFillStyle(0);
    legend->Draw("same");
  }
  for( const auto& line : lines_ ){
    assert(line);
    line->Draw("same");
  }
}
void HeapPicture::SetAxisTitles(const std::vector<std::string> &axis_titles) {
  assert(axis_titles.size()==2);
  axis_titles_ = axis_titles;
  auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1);
  stack_->SetTitle( title.c_str() );
}
HeapPicture::HeapPicture(const std::string &name,
                         const std::array<int, 2> &resolution)
    : Picture(name, resolution) {}
HeapPicture::~HeapPicture() {}
HeapPicture::HeapPicture() {}
