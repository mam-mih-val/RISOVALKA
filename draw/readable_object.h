//
// Created by mikhail on 5/6/21.
//

#ifndef FLOW_DRAWING_TOOLS_DRAW_READABLE_OBJECT_H_
#define FLOW_DRAWING_TOOLS_DRAW_READABLE_OBJECT_H_

#include <TFile.h>
#include <string>
#include <utility>
#include <vector>

class ReadableObject : public TObject {
public:
  ReadableObject() = default;
  ReadableObject(std::string file_name,
                 std::vector<std::string> objects)
      : file_name_(std::move(file_name)), objects_(std::move(objects)) {}
  ~ReadableObject() override = default;
protected:
  template <typename T>
  T* ReadObjectFromFile(const std::string& obj_name){
    if( !file_ )
      file_ = TFile::Open(file_name_.c_str() );
    if (!file_)
      throw std::runtime_error( "No such file "+ file_name_);
    T* obj{nullptr};
    file_->GetObject( obj_name.c_str(), obj );
    if( !obj )
      throw std::runtime_error( "No such object "+obj_name+" in file "+
          file_name_);
    return obj;
  }
  std::string file_name_;
  std::vector<std::string> objects_;
  TFile* file_{};
  ClassDefOverride(ReadableObject, 1)
};

#endif // FLOW_DRAWING_TOOLS_DRAW_READABLE_OBJECT_H_
