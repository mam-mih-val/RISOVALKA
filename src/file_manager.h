//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_
#define FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_

#include <iostream>

#include <TFile.h>

class FileManager {
public:
  template <class T>
  [[nodiscard]] static T* GetObject(std::string_view name){
    try {
      assert(Instance()->file_);
    } catch (const std::exception& e) {
      std::cout << "FileManager::GetCorrelation: File is not set" << std::endl;
      throw e;
    }
    T* obj;
    Instance()->file_->GetObject(name.data(), obj);
    try{
      assert(obj);
    } catch (const std::exception& e) {
      std::cout << "FileManager::GetCorrelation: No such a DataContainer called"
                << name << " in file" << std::endl;
      throw e;
    }
    return obj;
  }
  template <class T>
  [[nodiscard]] static std::vector<T*> GetObjectsVector(
      const std::vector<std::string_view>& names ){
    std::vector<T*> obj_vec;
    obj_vec.reserve(names.size());
    for( auto name : names ){
      obj_vec.emplace_back(GetObject<T>(name));
    }
    return obj_vec;
  }
  static void Open(const std::string& file_name);
private:
  static FileManager* instance_;
  static FileManager* Instance(){
    if (!instance_)
      instance_ = new FileManager();
    return instance_;
  }
  FileManager() = default;

  ~FileManager() = default;
  std::unique_ptr<TFile> file_{nullptr};
};

#endif // FLOW_DRAWING_TOOLS_SRC_FILE_MANAGER_H_
