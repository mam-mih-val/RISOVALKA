//
// Created by mikhail on 2/28/21.
//

#include "picture.h"

Picture::Picture(std::string name, const std::array<int, 2> &resolution)
    : name_(std::move(name)), resolution_(resolution) {
  canvas_ =  new TCanvas( name_.c_str(), "", resolution.at(0), resolution.at(1) );
  canvas_->SetBatch(true);
  std::string stack_name = name_+"_stack";
  stack_ =  new TMultiGraph(stack_name.c_str(), "");
  zero_line_ = new TF1( "zero_line", "0", -100, 100 );
}
void Picture::Save( const std::string& type ){
  assert(canvas_);
  auto save_name = name_+"."+type;
  canvas_->SaveAs( save_name.c_str() );
}

void Picture::Save( const std::string& name, const std::string& type ){
  assert(canvas_);
  auto save_name = name+"."+type;
  canvas_->SaveAs( save_name.c_str() );
}

void Picture::SavePoints(const std::string& name, const std::string& type){
  assert(stack_);
  auto save_name = name+"."+type;
  auto file_out = TFile::Open( save_name.c_str(), "recreate" );
  TGraph* graph;
  for( int i=0; i < stack_->GetListOfGraphs()->GetSize(); ++i ){
    graph = dynamic_cast<TGraph*>(stack_->GetListOfGraphs()->At(i));
    graph->Write( graph->GetTitle() );
  }
  //    stack_->SaveAs( save_name.c_str() );
  file_out->Close();
}
void Picture::CopyStyle(Picture* other) {
  resolution_ = other->resolution_;
  axis_titles_ = other->axis_titles_;
  draw_zero_line_ = other->draw_zero_line_;
  x_range_ = other->x_range_;
  y_range_ = other->y_range_;
  texts_ = other->texts_;
  text_sizes_ = other->text_sizes_;
  is_log_y_ = other->is_log_y_;
  is_log_x_= other->is_log_x_;
  is_log_z_ = other->is_log_z_;
}
