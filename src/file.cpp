#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdio>
#include <jpeglib.h>

#include "../include/file.h"
#include "../include/exception.h"

// #####################
// # getters and setters

unsigned int ImageInFile::getWidth() const{
    return m_width;
}

unsigned int ImageInFile::getHeight() const{
    return m_height;
}
