/**
 * @file   algorithm.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  Quick and dirty wrapper for c-style file management used in jpeglib
 */

#ifndef FILE_H
#define FILE_H

#ifndef NDEBUG
#include<iostream>
#endif

#include <cstdio>

/** 
 * file management is done C-style in libjpeg,
 * we wrap the FILE object with File to handle errors and close automatically.
 */
class File{
  
private:
    
  FILE* m_file = nullptr;
  
public:
  File(){}
  
  File( const std::string filePath, const std::string mode = "rb" ){
        m_file = std::fopen( filePath.c_str(), mode.c_str() );
	if ( m_file == nullptr ) {
	  throw("opening file "+filePath+" failed");
	}
  }
  
   FILE* getFile() const{
    return m_file;
  }
  
  ~File(){
        std::fclose( m_file );
  }
};

#endif
