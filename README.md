=========== 
LIBLOOKLIKE 
===========


REQUIREMENTS 
============

The following packages are required in order to build and use the
project files:

* cmake and pkg-config (if you choose POCL as OpenCL implementation)
* an opencl sdk an opencl library an icd loader (might be included in
  the opencl library)
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


Dependencies installation for debian
------------------------------------

The tools used to build the project can be obtained with:
    
    apt-get install build-essential libtool autoconf pkg-config \
        git cmake 

For debian with pocl implementation, run this:

    apt-get install ocl-icd-dev llvm clang hwloc \
        libhwloc-dev libarmadillo4 libarmadillo-dev mesa-common-dev \
        libjepeg-dev

then you need to install opencl manually:

    git clone https://github.com/pocl/pocl.git cd pocl sh autogen.sh

At this step, you need to know what sort of cpu is installed on your
computer. You should select the closest match in the list given by the
following command:

    llvm-as < /dev/null -o - | llc -mcpu=help

For this example we suppose the cpu is from the intel core-i\* family,
so we choose the cpu option corei7.

    export LLC_HOST_CPU=corei7 
    ./configure
    make
    sudo make install


TEST ENVIRONMENT 
================

Sample images 
-------------

This software was developped using essentialy the image set from this
webiste: http://fei.edu.br/~cet/facedatabase.html

To run the tests, you can use the centered portraits.  In the project
directory, run:

    cd data 
    wget http://fei.edu.br/~cet/frontalimages_manuallyaligned_part1.zip 
    unzip frontalimages_manuallyaligned_part1.zip

To keep only the neutral faces:

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

Retrieve project
----------------

To download the lastest version of the project, use the git repository.

    git clone https://github.com/pixelou/liblooklike.git

For the rest rest of this file, we assume the project directory is in the
variable ${LLL_ROOT} .

You can set this variable just after cloning the repo by issuing:

    export LLL_ROOT=`pwd`/liblooklike

Preparation
-----------

In order to build the project, you need to create a build directory:

    mkdir build && cd build

generate makefiles with cmake:

    cmake ${LLL_ROOT}

If you prefer to use use POCL and skip documentation generation:

    cmake ${LLL_ROOT} -DUSE_POCL=TRUE -DBUILD_DOCUMENTATION=FALSE

Building
--------

You can build everything with:

    make

Optionnaly, run the tests:

    make test

Targets
-------

* binary files will be generated in output/bin
* tests in tests/bin
* documentation in output/doc
* libraries in output/lib


USAGE
=====

img2eigenvec
------------

img2eigenvec generates the eigenvectors from a set of images.
Supposing the pictures are in ${LLL_ROOT}/data, you can run:

    img2eigenvec ${LLL_ROOT}/kernels ${LLL_ROOT}/data

It will generate previews of the eigen vectors as jpeg files in /tmp


