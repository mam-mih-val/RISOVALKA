//
// Created by Misha on 4/29/2023.
//

#ifndef RISOVALKA_FILE_MANAGER_H
#define RISOVALKA_FILE_MANAGER_H

#include <map>
#include <bits/unique_ptr.h>
#include <TFile.h>

class FileManager {

private:
public:
  static FileManager* Instance(){
    if(!instance_)
      instance_ = std::unique_ptr<FileManager>( new FileManager );
    return instance_.get();
  }
  ~FileManager() = default;

  static TFile* OpenFile( const std::string& file_name ){
    try{
      return Instance()->opened_files_.at(file_name).get();
    }catch ( std::exception& ){
      auto [pair, success] = Instance()->opened_files_.insert( std::make_pair( file_name, TFile::Open(file_name.c_str())  ) );
      if( pair->second )
        return pair->second.get();
    }
    throw std::runtime_error( "File with the name "+file_name+" is not found." );
  }

  template<class T>
  static T* ReadObject( const std::string& file_name, const std::string& obj_name ){
    auto file = OpenFile( file_name );
    T* obj{nullptr};
    file->GetObject( obj_name.c_str(), obj );
    if( obj ) return obj;
    throw std::runtime_error( "Object with the name "+obj_name+" is not found" );
  }

private:
  FileManager() = default;
  static std::unique_ptr<FileManager> instance_;
  std::map< std::string, std::unique_ptr<TFile> > opened_files_{};

};


#endif //RISOVALKA_FILE_MANAGER_H
