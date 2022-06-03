//
// Created by mikhail on 6/3/22.
//

#ifndef RISOVALKA_DRAW_BOOK_H_
#define RISOVALKA_DRAW_BOOK_H_

#include <vector>
#include "picture.h"

class Book {
public:
  Book() = default;
  void SavePDF(const std::string& file_name);
  void SaveRoot(const std::string& file_name);
  void ApplyLayoutAll(){ for( auto pic : canvases_ ){ pic->CopyStyle(layout_); } }
  void SetLayout(Picture *layout) { layout_ = layout; }
  void AddPage( Picture* pic, bool apply_layout=true ){
    canvases_.push_back( pic );
    if( apply_layout && layout_ )
      pic->CopyStyle(layout_);
  }

protected:
  std::vector<Picture*> canvases_;
  Picture* layout_{nullptr};
};

#endif // RISOVALKA_DRAW_BOOK_H_
