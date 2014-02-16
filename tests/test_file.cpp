/**
 * @file   test_context.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Context class
 */

#include<string>
#include<iostream>
#include<cstdio>

#include "../include/jpegfile.h"
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
            if( ch != *ref++ )
                throw Error("Cannot recognise sample file");
        }

        std::cout << "testing simple image file opening\n";
        std::string filePath = std::string(argv[argc-2]) + "/100a.jpg";
        JPEGImageInFile fin(filePath);
        JPEGImageOutFile fout;
        float* ImageData = new float[fin.getHeight()*fin.getWidth()];
        fin.load(ImageData,fin.getWidth(),fin.getHeight());
        fout.write(ImageData,
                             fin.getWidth(),fin.getHeight(),
                             "test.jpg");
        delete[] ImageData;
        std::cout << "testing array of files load\n";
        ArrayOfImages m = arrayOfImagesFromFiles( std::string(argv[argc-2]) );
    } catch( Error& e ) {
        e.printMsg();
        return -1;
    }
    return 0;
}
