Tutorials {#tutorials}
=========

@brief Presentation of the basics with a simple computation program


Using expressions and algorithms
================================

The following example illustrate how to use the library to perform the
computation of an expression as a result of several algorithm chained
together.

It can be compile using a command similar to:

    $(CXX) tutorial1.cpp real.cpp realoperations.cpp \
    -llooklike -L/path/to/liblooklike.a \
    -I../../include/ -std=c++11

@include tutorial1.cpp


Creating expressions
====================

Below is an example of expression for real values.

The example can be compiled with a command similar to:

    $(CXX) tutorial2.cpp real.cpp realoperations.cpp \
    -llooklike -lpthread -L/path/to/liblooklike.a \
    -I../../include/ -std=c++11

real.h:
@include developing/real.h

real.cpp:
@include developing/real.cpp


Creating algorithms
===================

Below is an example of algorithm for sum of real values.

realoperations.h:
@include developing/realoperations.h

realoperations.cpp:
@include developing/realoperations.cpp


Using OpenCL backend
====================

Using OpenCL requires a few extra steps in order to setup the environment.
We use the Context wrapper for that purpose.

This example can be compiled using a command like:

    clang++ -g clexample.cpp \
    -std=c++11 -llooklike -lOpenCL \
    -L/path/to/liblooklike.a \
    -I../../include/

clexample.cpp:
@include clexample.cpp
