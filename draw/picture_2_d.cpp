//
// Created by mikhail on 5/14/21.
//

#include "picture_2_d.h"
void Picture2D::SetAxisTitles(const std::vector<std::string> &axis_titles) {
  assert(axis_titles.size()==2);
  axis_titles_ = axis_titles;
  auto title = ";"+axis_titles.at(0)+";"+axis_titles.at(1)+";";
  histogram_->SetTitle( title.c_str() );
}

void Picture2D::Draw() {
  this->Picture::Draw();
  canvas_->cd();
  histogram_->Draw(draw_option_.c_str());
  if(is_log_x_)
    gPad->SetLogx();
  if(is_log_y_)
    gPad->SetLogy();
  if(is_log_z_)
    gPad->SetLogz();
  if( x_range_.at(0) < x_range_.at(1) ) {
    histogram_->GetXaxis()->SetRangeUser(x_range_.at(0), x_range_.at(1));
    histogram_->Draw(draw_option_.c_str());
  }
  if( y_range_.at(0) < y_range_.at(1) ) {
    histogram_->GetYaxis()->SetRangeUser(y_range_.at(0), y_range_.at(1));
    histogram_->Draw(draw_option_.c_str());
  }
  if( z_range_.at(0) < z_range_.at(1) ) {
    histogram_->SetMinimum(z_range_.at(0));
    histogram_->SetMaximum(z_range_.at(1));
    histogram_->Draw(draw_option_.c_str());
  }
  int i=0;
  for( auto text : texts_ ){
    text->SetNDC();
    text->SetTextSize(text_sizes_.at(i));
//    text->SetLineWidth(1);
//    text.SetLineColor(kBlack);
    text->Draw("same");
    ++i;
  }
}
void Picture2D::CopyStyle(Picture2D* other) {
  this->Picture::CopyStyle(other);
  draw_option_ = other->draw_option_;
}
