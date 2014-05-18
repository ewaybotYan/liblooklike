#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <dirent.h>
#include <string>
#include <deque>
#include <memory>

#include "../include/jpegfile.h"
#include "../include/simplematrix.hpp"
#include "../include/exception.h"
#include "../include/image.h"

using namespace std;


// #######################
// # Functions declaration

ArrayOfImages arrayOfImagesFromFiles (const std::string path)
{
  DIR *dir;
  struct dirent *ent;
  std::deque<JPEGImageInFile> imagefiles;

  // filenames backup for later identification
  shared_ptr< vector<string> > fileNames =
      make_shared< vector<string> >();

  // first, get all jpeg files fro path
  if ( ( dir = opendir ( path.c_str() ) ) == NULL )
    throw Error ( "could not open specified directory" );

  while ( ( ent = readdir ( dir ) ) != NULL ) {
    std::string fileName ( ent->d_name );
    if ( ent->d_type != DT_REG )
      continue;
    try {
      imagefiles.push_back ( path+"/"+fileName );
      fileNames->push_back(string(ent->d_name));
    } catch ( Error const& e ) {
      // that was not a valid JPEG file, just skip it
    }
  }
  closedir ( dir );

  // then get the average size
  unsigned int avgWidth = 0;
  unsigned int avgHeight = 0;
  for ( JPEGImageInFile f : imagefiles ) {
    avgWidth += f.getWidth();
    avgHeight += f.getHeight();
  }
  avgWidth /= imagefiles.size();
  avgHeight /= imagefiles.size();
#ifndef NDEBUG
  std::cout << "chosen image size is : " << avgWidth << "x" << avgHeight << "\n";
#endif

  // create result object
  shared_ptr< vector<cl_float> > values =
      make_shared< vector<cl_float> >( avgWidth*avgHeight*imagefiles.size() );

  //then load the pixels from these files
  int offset = 0;
  cl_float* data = values->data();
  for ( JPEGImageInFile f : imagefiles ) {
    f.load( data + offset, avgWidth, avgHeight );
    offset+= avgHeight * avgWidth;
  }

  ArrayOfImages res = {
    avgWidth,
    avgHeight,
    imagefiles.size(),
    values,
    fileNames
  };
#ifndef NDEBUG
  cout << imagefiles.size() << " images of size "
       << avgHeight << "x" << avgWidth << endl;
#endif
  return res;
}


void MatrixToImage( SimpleMatrix<cl_float>& src,
                    unsigned int h,
                    unsigned int w,
                    unsigned int column,
                    bool onCols,
                    const std::string savePath ) {

  // scale data
  float minVal = src.at(0,column);
  float maxVal = src.at(0,column);
  float avg=0;
  for( unsigned int i= 0; i < w*h; i++){
    if( onCols ){
      minVal = std::min(minVal, src.at(i,column));
      maxVal = std::max(maxVal, src.at(i,column));
      avg += src.at(i,column);
    }else{
      minVal = std::min(minVal, src.at(column,i));
      maxVal = std::max(maxVal, src.at(column,i));
      avg += src.at(i,column);
    }
  }
  avg/=w*h;
#ifndef NDEBUG
  std::cout << "vector rng : " << minVal << " " << avg << " " << maxVal << "\n";
#endif
  vector<cl_float> scaled(w*h);
  if( onCols ){
  for( unsigned int i= 0; i < w*h; i++)
    scaled[i] = (src.at(i,column) - minVal)*255/(maxVal-minVal);
  }else{
    for( unsigned int i= 0; i < w*h; i++)
      scaled[i] = (src.at(column,i) - minVal)*255/(maxVal-minVal);
  }

  // save it
  JPEGImageOutFile fout;
  fout.write( scaled.data(),
              w,
              h,
              savePath);
}


unsigned int ImageInFile::getWidth() const {
  return m_width;
}


unsigned int ImageInFile::getHeight() const {
  return m_height;
}
