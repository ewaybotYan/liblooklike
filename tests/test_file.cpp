/**
 * @file   test_context.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Context class
 */

#include<string>
#include<iostream>

#include "../include/image.h"
#include "../include/exception.h"


using namespace std;

void usage(){
  cerr << "usage :\n test_file " 
    << endl;
}

int main( const int argc, const char* argv[] ){
    if( argc != 2 ){
        usage();
        return -1;
    }
    try{
    Matrix m = arrayOfImagesFromFiles( std::string(argv[argc-1]) );
    }catch( Error& e ){
        e.printMsg();
    }
    return 0;
}