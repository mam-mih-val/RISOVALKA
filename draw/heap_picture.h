//
// Created by mikhail on 3/6/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
#define FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_

#include "picture.h"

class HeapPicture : public Picture {
public:
  HeapPicture();
  HeapPicture(const std::string &name, const std::array<int, 2> &resolution);
  ~HeapPicture() override;
  void AddDrawable( DrawableObject* obj ){
    assert(obj);
    drawable_objects_.push_back( std::unique_ptr<DrawableObject>(obj) );
  }
  void AddDrawable( std::unique_ptr<DrawableObject>&& obj ){
    assert(obj);
    if( obj->IsLine() ){
      std::string opt{"L+" + obj->GetErrorOption()};
      stack_->Add(obj->GetPoints(), opt.c_str());
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"L");
      if( obj->GetSysErrorPoints() )
        stack_->Add( obj->GetSysErrorPoints(), "L+2" );
    } else{
      std::string opt{"P+" + obj->GetErrorOption()};
      stack_->Add(obj->GetPoints(), opt.c_str());
      if( auto_legend_ )
        legends_.back()->AddEntry(obj->GetPoints(), obj->GetTitle().c_str(),"P");
      if( obj->GetSysErrorPoints() )
        stack_->Add( obj->GetSysErrorPoints(), "P+2" );
    }
//    drawable_objects_.push_back( std::move(obj) );
  }
  void AddDrawables( const std::vector<DrawableObject*>& objects ){
    for( auto obj : objects ){
      this->AddDrawable(obj);
    }
  }
  void SetAxisTitles(const std::vector<std::string> &axis_titles) override;
  void Draw() override;

protected:
  std::vector< std::unique_ptr<DrawableObject> > drawable_objects_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
