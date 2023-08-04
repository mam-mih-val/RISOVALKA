//
// Created by mikhail on 6/3/22.
//

#include <TPDF.h>
#include <TPostScript.h>
#include "book.h"

void Book::SavePDF(const std::string& file_name){
  auto pdf = new TPDF(file_name.data());
  auto save_name = file_name+"[";
  for( const auto& canv : canvases_ ){
    if( !canv->IsDrawn() )
      canv->Draw();
    canv->GetCanvas()->Update();
  }
  pdf->Close();
}
void Book::SavePostscript(const std::string &file_name) {
  auto p_script = new TPostScript(file_name.data());
  auto save_name = file_name+"[";
  for( const auto& canv : canvases_ ){
    if( !canv->IsDrawn() )
      canv->Draw();
    canv->GetCanvas()->Update();
  }
  p_script->Close();
}
void Book::SaveRoot(const std::string &file_name) {
  auto file_out = TFile::Open( file_name.c_str(), "recreate" );
  file_out->cd();
  for( const auto& canv : canvases_ ){
    if( !canv->IsDrawn() )
      canv->Draw();
    canv->GetCanvas()->Write("name");
  }
  file_out->Close();
}
