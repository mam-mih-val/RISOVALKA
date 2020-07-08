//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_

#include <iostream>

#include <TFile.h>

class FileManager {
public:
  static FileManager* Instance(){
    if (!instance_)
      instance_ = new FileManager();
    return instance_;
  }
  template <class T>
  [[nodiscard]] T* GetObject(std::string_view name){
    if( !file_ ){
      std::cout << "FileManager::GetCorrelation: File is not set" << std::endl;
      abort();
    }
    T* obj;
    file_->GetObject(name.data(), obj);
    if( !obj) {
      std::cout << "FileManager::GetCorrelation: No such a DataContainer called"
                << name << " in file" << std::endl;
      abort();
    }
    return obj;
  }
  template <class T>
  [[nodiscard]] std::vector<T*> GetObjectsVector(
      const std::vector<std::string_view>& names ){
    std::vector<T*> obj_vec;
    obj_vec.reserve(names.size());
    for( auto name : names ){
      obj_vec.emplace_back(GetObject<T>(name));
    }
    return obj_vec;
  }
  void Open(const std::string& file_name){
    file_.reset(TFile::Open(file_name.data()));
    if( !file_ ){
      std::cerr << "Error in FileManager::Open()" << file_name << std::endl;
      std::cerr << "No such a file " << file_name << std::endl;
      abort();
    }
  };
private:
  FileManager() = default;
  ~FileManager() = default;

  static FileManager* instance_;
  std::unique_ptr<TFile> file_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_
