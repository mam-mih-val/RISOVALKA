//
// Created by mikhail on 2/28/21.
//

#include "picture.hpp"
ClassImp(Picture);

void Picture::Draw(){
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
    stack_->GetXaxis()->SetLimits(x_range_.at(0), x_range_.at(1));
    stack_->Draw();
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    stack_->GetYaxis()->SetLimits(y_range_.at(0), y_range_.at(1));
    stack_->Draw();
  }
  for( auto text : texts_ ){
    text->SetNDC();
    text->SetTextSize(0.04);
//    text->SetLineWidth(1);
//    text.SetLineColor(kBlack);
    text->Draw("same");
  }
  for(auto legend : legends_) {
    assert(legend);
    legend->Draw("same");
  }
}