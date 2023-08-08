//
// Created by mikhail on 5/7/22.
//

#ifndef RISOVALKA_DRAW_PALETTE_H_
#define RISOVALKA_DRAW_PALETTE_H_

#include <TStyle.h>

#include <utility>

#include "drawable_object.h"

class Palette{
public:
  Palette() = default;
  virtual ~Palette() = default;
  void SetMarkers(std::vector<int> markers) { markers_ = std::move(markers); }
  [[deprecated( "SetMarker is deprecated. Use SetMarkers() instead" )]]
  void SetMarker(int marker) {
    markers_.at(0) = marker; }
  void SetPalette(const std::vector<int> &palette) { palette_ = palette; }
  void BiasPalette( bool bias_palette = true ){ bias_palette_ = bias_palette; }
protected:
  template<class T>
  void ColorObjects(const std::vector<T>& objects){
//    if( !std::is_base_of<DrawableObject, T>::value )
//      throw std::runtime_error( __func__ + std::string(" vector of objects is not based of DrawableObject") );
    std::vector<int> colors;
    if( objects.size() < palette_.size() && bias_palette_ ){
      auto bias = palette_.size() / objects.size();
      for( size_t i=0; i<objects.size(); ++i )
        colors.push_back(palette_.at(i*bias));
    }else{
      size_t bias=0;
      size_t position=0;
      while( objects.size() > colors.size() ){
        if( position >= palette_.size() ){
          position=0;
          bias++;
        }
        colors.push_back( palette_.at( position )+bias );
        position++;
      }
    }
    int i=0;
    for( const auto& obj : objects ){
      if( markers_.size() != 1 ) {
        try {
          obj->SetStyle(colors.at(i), markers_.at(i));
        }catch(std::exception& e) {
          throw std::runtime_error( __func__  + std::string(" vector of markers is shorter than a vector of Drawables") );
        }
      } else
        obj->SetStyle(colors.at(i), markers_.at(0));
      ++i;
    }
  }

  std::vector<int> markers_{kFullCircle};
  std::vector<int> palette_{
      kPink,
      kMagenta+1,
      kViolet-8,
      kBlue,
      kAzure-4,
      kCyan+1,
      kGreen+2,
      kSpring-4,
      kYellow-3,
      kOrange-3,
      kRed,
  };
  bool bias_palette_{true};
};

#endif // RISOVALKA_DRAW_PALETTE_H_
