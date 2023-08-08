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
  void AddDrawable( DrawableObject* obj );
  void AddDrawable( const std::unique_ptr<DrawableObject>& obj ) ;
  void AddDrawables( const std::vector<DrawableObject*>& objects );
  void SetAxisTitles(const std::vector<std::string> &axis_titles) override;
  void Draw() override;

protected:
  std::vector< std::unique_ptr<DrawableObject> > drawable_objects_;
};

#endif // FLOW_DRAWING_TOOLS_DRAW_HEAP_PICTURE_H_
