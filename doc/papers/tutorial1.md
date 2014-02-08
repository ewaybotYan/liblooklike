% Tutorial 1 : Introduction to paralellized environments, The example of OpenCL Nicolas Granger
% draft


Note to readers 
===============


Introduction 
============


A short definition of parallelization 
-------------------------------------

The word parallelization is becoming very common in Computer Science, and has spread outside
laboratories. Intense computation tasks are widely used in various domains outsides science : games,
picture editing, pattern recognition... 

To simplify, we can consider two of the fields of study in computer science:
- the improvement of computer performances 
- the design of more optimized algorithms

Parallelization bridges both of these orientations. It addresses the limitation of the standard
computers by multiplicating the computation devices inside the machine, but requires specifically
designed algorithms, that can split work among the devices instead of queuing tasks on a single one.
Thus if you replicate six times your computation devices and split work equally between each one,
you can expect a performance improvement by a factor up to six.

Figure [] shows the diffrence between sequential, and parallel execution.

Further in this serie of tutorials, we will call computation device the object that take data input,
execute a computation on it and returns the result of the algorithm. Parallelization will happen
inside this computation device.


The specific role of the GPU 
----------------------------

If the problem of parallelization is indeed to create a compatible algorithm, the existence of
physical architectures providing the assets to run the programs is in fact quite recent. The first
solution was to build a pool of computers inside a network, and develop a software layer on top to
syncronise tasks and memory operations. Graphic cards have lately come into the field of
computation devices.
They actually are all-in-one parallel computation devices boasting several thousands of
microprocessors optimised for float computation and several types of memory. They provide higher
computation efficency than a classical CPU who are designed for more complex tasks.


OpenCL reference 
----------------

The diversity of graphic cards and more generally of devices with parallelization abilities is wide.
As a consequence, a uniform programming interface had to be designed in order to provide a common
standard for parallel computation. This enables developpers to provide a single code that can be run
on a wide range of devices.

The solution we will explore here is called OpenCL, but this is not the only one (CUDA (tm) is
another example).

OpenCL is a specification. This specification  was not published by a graphic card manufacturer, but
rather by the Chronos Group who designed it has had a long relationship with the graphic cards
market.  Being a specification, it only defines a set of functions with a description of the
expected behaviour. The purpose of these functions is to handle the operations needed to execute a
computation on a device. Thus you can find functions ranging from devices listing to data and
programs loading.  As of now, the specification has been implemented by the main desktop computer
part makers, but also for high end embedded devices such as cellphones.  The libraries however do
not always implement all the functionalities of the specification.
