% Tutorial 1 : Introduction to paralellized environments, The example of OpenCL
% Nicolas Granger
% draft


Note to readers
===============

Please, keep in mind that this document is still a draft version.

Introduction
============

A short definition of parallelization
-------------------------------------

The word parallelization is becoming very common in Computer Science, and has spread outside laboratories. Intense computation tasks are widely used in various domains outsides science : games, picture editing... 

To simplify, we can consider two of the fields of study in computer science:
- the improvement of computer performances 
- the design of more optimized algorithms

Parallelization bridges both of these orientations. It addresses the limitation of the standard computers by multiplicating the computation devices inside the machine, but requires specifically designed algorithms, that can split work among the devices instead of queuing tasks on a single one.
Thus if you replicate six times your computation devices and split work equally between each one, you can expect a performance improvement by a factor up to six.

Figure [] shows the diffrence between sequential, and parallel execution.


The specific role of the GPU
----------------------------

If the problem of parallelization is indeed to create a compatible algorithm, the existence of physical architectures providing the assets to run the programs is in fact quite recent. The first solution was to build a pool of computers inside a network, and develop software layer to syncronise tasks and memory operations.
Graphic cards have lately come into the field of computation devices. They actually are all-in-one parallel computation devices boasting several thousands of microprocessors optimised for float computation. They provide higher computation efficency than CPU, who are designed for more complex tasks.

OpenCL reference
----------------

OpenCL is a specification, it was not designed by a graphic card manufacturer, but the Chronos Group who designed it has had a long relationship with the graphic cards market.
Being a specification, it only defines a set of functions with a description of the expected behaviour. The purpose of these functions is to handle the operations needed to execute a computation on a device. Thus you can find functions ranging from devices listing to data and programs loading.
As of now, the specification has been implemented by the main desktop Computer makers, but also for high end embedded devices such as cellphones. The libraries however do not always implement all the functionalities.

