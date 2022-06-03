//
// Created by mikhail on 6/3/22.
//

#include "book.h"

void Book::SavePDF(const std::string& file_name){
  auto save_name = file_name+"[";
  canvases_.front()->GetCanvas()->Print(save_name.c_str());
  for( auto canv : canvases_ ){
    if( !canv->IsDrawn() )
      canv->Draw();
    canv->GetCanvas()->Print(file_name.c_str());
  }
  save_name = file_name+"]";
  canvases_.back()->GetCanvas()->Print(save_name.c_str());
}
void Book::SaveRoot(const std::string &file_name) {
  auto file_out = TFile::Open( file_name.c_str(), "recreate" );
  file_out->cd();
  for( auto canv : canvases_ ){
    if( !canv->IsDrawn() )
      canv->Draw();
    canv->GetCanvas()->Write("name");
  }
  file_out->Close();
}
