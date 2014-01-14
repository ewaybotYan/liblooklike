/**
 * @file   test_context.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Context class
 */

#include<string>
#include<iostream>
#include<cstdio>

#include "../include/image.h"
#include "../include/file.h"
#include "../include/exception.h"


using namespace std;

std::string sampleTxt = "This is a sample text.";

void usage () {
  cerr << "usage :\n test_file " 
    << endl;
}

int main ( const int argc, const char* argv[] ) {
    if ( argc < 3 ){
        usage();
        return -1;
    }
    try {
        std::cout << "testing simple file opening\n";
        File f(argv[argc-1]);
        FILE* F = f.getFile();
        char ch;
        char* ref = (char*)sampleTxt.c_str();
        while((ch=std::fgetc(F))!=EOF && ch != '\n' && ch != '\0'){
#ifndef NDEBUG
            std::cout << ch << "\n";
#endif
            if( ch != *ref++ )
                throw Error("Cannot recognise sample file");
        }

        Matrix m = arrayOfImagesFromFiles( std::string(argv[argc-2]) );
    } catch( Error& e ) {
        e.printMsg();
        return -1;
    }
    return 0;
}
