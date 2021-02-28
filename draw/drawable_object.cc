//
// Created by mikhail on 2/28/21.
//

#include "drawable_object.h"
void DrawableObject::SetMarkerStyle() {
  points_->SetTitle(title_.c_str());
  if( marker_ > 0 )
    points_->SetMarkerStyle(marker_);
  else
    points_->SetLineStyle(-marker_);
  points_->SetMarkerColor(color_);
  points_->SetLineColor(color_);
}
