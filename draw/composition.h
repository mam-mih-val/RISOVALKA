//
// Created by Misha on 8/8/2023.
//

#ifndef RISOVALKA_COMPOSITION_H
#define RISOVALKA_COMPOSITION_H

#include "drawable_object.h"

template<class T>
class Composition : public DrawableObject{
public:
  Composition() : DrawableObject() { };
  Composition(const std::string& title) { title_ = title; }
  template<typename... Args>
  void Add( Args... args ){ vec_objects_.emplace_back( args... ); }
  template<typename Func>
  // Func function should return the <std::vector, std::vector> with values for y and y_er
  void Compose( const std::vector<double>& x_axis, Func points_retriever ){
    std::vector<double> y_axis{};
    std::vector<double> x_err{};
    std::vector<double> y_err{};
    for( const auto& obj : vec_objects_ ){
      auto [obj_y_axis, obj_y_err] = points_retriever( obj );
      y_axis.insert( y_axis.end(), obj_y_axis.begin(), obj_y_axis.end() );
      y_err.insert( y_err.end(), obj_y_err.begin(), obj_y_err.end() );
    }
    points_ = std::make_unique<TGraphErrors>( x_axis.size(), x_axis.data(), y_axis.data(), nullptr, y_err.data() );
    this->SetMarkerStyle();
  }

  template<typename Func>
  void Perform( Func function ){ std::for_each( vec_objects_.begin(), vec_objects_.end(), function ); }

private:
  std::vector<T> vec_objects_{};
};


#endif //RISOVALKA_COMPOSITION_H
