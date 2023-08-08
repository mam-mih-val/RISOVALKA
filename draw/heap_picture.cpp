//
// Created by mikhail on 3/6/21.
//

#include "heap_picture.h"
#include "TH1.h"

void HeapPicture::Draw() {
  this->Picture::Draw();
  if(auto_legend_){
    assert(legends_.empty());
    legends_.emplace_back( new TLegend() );
  }
//  for( const auto& obj : drawable_objects_ ){
//    if( obj->IsLine() ) {
//      std::string opt{"L+" + obj->GetErrorOption()};
//      stack_->Add(obj->GetPoints(), opt.c_str());
//      if( auto_legend_ )
//        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"L");
//      if( obj->GetSysErrorPoints() )
//        stack_->Add( obj->GetSysErrorPoints(), "L+2" );
//    } else {
//      std::string opt{"P+" + obj->GetErrorOption()};
//      stack_->Add(obj->GetPoints(), opt.c_str());
//      if( auto_legend_ )
//        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"P");
//      if( obj->GetSysErrorPoints() )
//        stack_->Add( obj->GetSysErrorPoints(), "P+2" );
//    }
//  }
  canvas_->cd();
  if(is_log_x_)
    gPad->SetLogx();
  if(is_log_y_)
    gPad->SetLogy();
  stack_->Draw("APL");
  if( drawable_objects_.empty() ) {
    functions_.at(0)->Draw();
    if( x_range_.at(0) < x_range_.at(1) ) {
      functions_.at(0)->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
      zero_line_->SetRange(x_range_.at(0), x_range_.at(1));
      functions_.at(0)->Draw();
    }
    if( y_range_.at(0) < y_range_.at(1) ) {
      functions_.at(0)->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
      functions_.at(0)->Draw();
    }
    for( size_t i=1; i<functions_.size(); ++i ){
      functions_.at(i)->Draw("same");
    }
  } else {
    for (const auto& func : functions_) {
      if (func)
        func->Draw("same");
    }
  }
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
//    text->SetLineWidth(1);
//    text.SetLineColor(kBlack);
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
