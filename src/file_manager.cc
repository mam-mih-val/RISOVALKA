//
// Created by mikhail on 6/21/20.
//

#include "file_manager.h"

FileManager* FileManager::instance_ = nullptr;

void FileManager::Open(const std::string& file_name){
  Instance()->file_.reset(TFile::Open(file_name.data()));
  if( !Instance()->file_ ){
    std::cerr << "Error in FileManager::Open()" << file_name << std::endl;
    std::cerr << "No such a file " << file_name << std::endl;
    abort();
  }
};