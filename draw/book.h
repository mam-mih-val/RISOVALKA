//
// Created by mikhail on 6/3/22.
//

#ifndef RISOVALKA_DRAW_BOOK_H_
#define RISOVALKA_DRAW_BOOK_H_

#include <vector>
#include "picture.h"
#include "heap_picture.h"

class Book {
public:
  Book() = default;
  ~Book() = default;
  void SavePDF(const std::string& file_name);
  void SavePostscript(const std::string& file_name);
  void SaveRoot(const std::string& file_name);
  void ApplyLayoutAll(){ for( const auto& pic : canvases_ ){ pic->CopyStyle(layout_.get()); } }
  void SetLayout(Picture *layout) { layout_.reset(layout); }
  void AddPage( Picture* pic, bool apply_layout=true ){
    canvases_.push_back( std::unique_ptr<Picture>(pic) );
    if( apply_layout && layout_ )
      pic->CopyStyle( layout_.get() );
  }

protected:
  std::vector<std::unique_ptr<Picture>> canvases_;
  std::unique_ptr<Picture> layout_;
};

#endif // RISOVALKA_DRAW_BOOK_H_
