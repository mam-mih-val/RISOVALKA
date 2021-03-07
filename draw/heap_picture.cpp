//
// Created by mikhail on 3/6/21.
//

#include "heap_picture.h"
ClassImp(HeapPicture)

void HeapPicture::Draw() {
  if(auto_legend_){
    assert(legends_.empty());
    legends_.emplace_back( new TLegend() );
  }
  for( auto obj : drawable_objects_ ){
    if( obj->IsLine() ) {
      stack_->Add(obj->GetPoints(), "L");
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"L");
    } else {
      stack_->Add(obj->GetPoints(), "P");
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"P");
    }
  }
  canvas_->cd();
  stack_->Draw("AP");
  if( x_range_.at(0) < x_range_.at(1) ) {
    stack_->GetXaxis()->SetRangeUser(x_range_.at(0), x_range_.at(1));
    stack_->Draw();
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    stack_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
    stack_->Draw();
  }
  if( draw_zero_line )
    zero_line_->Draw("same");
  int i=0;
  for( auto text : texts_ ){
    text->SetNDC();
    text->SetTextSize(text_sizes_.at(i));
//    text->SetLineWidth(1);
//    text.SetLineColor(kBlack);
    text->Draw("same");
    ++i;
  }
  for(auto legend : legends_) {
    assert(legend);
    legend->Draw("same");
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
