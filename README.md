=========== 
LIBLOOKLIKE 
===========


REQUIREMENTS 
============

The following packages are required in order to build and use the
project files:

* cmakei and pkg-config (if you choose POCL as OpenCL implementation)
* an opencl sdk an opencl library an icd loader (might be included in
* the opencl library)

There are several providers for this triplet, nvidia, amd and
optionnaly intel.
* Nvidia opencl library only support Nvidia hardware. It is likely to
  be package for your distribution because the licence permits it.
* AMD's has support for both AMD graphic cards and any sse3 compatible
  processor. You should choose this if you don't have an Nvidia 
  graphic card. Note that AMD license prevents most linux distribution
  to package it in the repositories.
* POCL is simpler, *ideal for developpement and testing*. 
* Intel sdk for OpenCL not available on linux.


dependencies installation for debian
------------------------------------

For debian with pocl implementation, run this

    sudo apt-get install build-essential libtool autoconf pkg-config
    git cmake sudo apt-get install ocl-icd-dev llvm clang hwloc \
        libhwloc-dev libarmadillo4 libarmadillo-dev mesa-common-dev \
        libjepeg-dev

then you need to install opencl manually:

    git clone https://github.com/pocl/pocl.git cd pocl sh autogen.sh

At this step, you need to know what sort of cpu is installed on your
computer. You should select the closest match in the list given by the
following command:

    llvm-as < /dev/null -o - | llc -mcpu=help

For this example we suppose the cpu is from the intel core-i* family,
so we choose the cpu option corei7.

    export LLC_HOST_CPU=corei7 
    ./configure make sudo make install


TEST ENVIRONMENT 
================

Sample images 
-------------

This software was mostly developped using the image set from this
webiste: http://fei.edu.br/~cet/facedatabase.html

to run the tests, you can use the centered portraits.  In the project
directory, run:

    cd data 
    wget http://fei.edu.br/~cet/frontalimages_manuallyaligned_part1.zip 
    unzip frontalimages_manuallyaligned_part1.zip

to keep only the neutral faces:

    rm *b.jpg

Tests 
-----

Tests programs are available in tests/ .  
They are a good entry point to the project and can be read as 
examples. Tests are build along the rest if the project. To run the 
tests, execute the build steps detailed below and then in the build 
directory run:

    make test


BUILDING 
========

In order to build the project, you need to create a build directory:

 mkdir build && cd build

generate makefiles with cmake:

  cmake (path to the project root dir)

if you use a build directory at the root of the project and use POCL
statically (without icd loader support), you may type:

cmake path/to/liblooklike 
  
cmake ../ -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="-Werror -Wall -Wextra -Wunreachable-code"

then build everything:

  make 

Optionnaly, run the tests:

  make test
