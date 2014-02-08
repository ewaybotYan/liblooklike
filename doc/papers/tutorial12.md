The overall process as defined in OpenCL
========================================

The OpenCL specification divides the computation process into several parts.
We will present each of them below, and introduce the naming convention used in the specification.


choosing a platform
-------------------

The specification is implemented into a library, that is to say a piece of computer program running
on both the computer and the computation device.
The first step is to choose which platform implementation will be used if several ones are available.
Since most computation devices manufacturer provide a library for their own devices, you usually end
up choosing the library that matches the devices on wich we want to work. 

Ex: If you wish to work with an AMD graphic card, you will want to use AMD's OpenCL implementation, 
wich is the only one available at the time of writing.

Note that a platform can handle multiple devices at the time, provided that all of them are
supported by the implementation. Thus you can imagine a computation device that is actually an
aggregation of several identical graphic cards.


starting a context
------------------

A context has several roles, it manages memory buffers, devices and command queues. The context in a
Object oriented programming point of view is what matches best what we have called computation
device so far. It is the root object during the whole computation process, wich means it handles devices, 
memory, buffers and command queues. 


The context is usually created from a list of devices listed by the platform selected during the
previous step.


allocating memory
-----------------

The memory model of OpenCL is more complicated than in a stadard computer. For example, data can be
split or shared implicitly between devices. More generally there is an abstraction layer between the
real memory management inside the computation device and the OpenCL interface. The purpose of this
complex system is to make parallelization possible in the device without low level hardware 
considerations, as everything is automatically handled by the OpenCL implementation.
Data transfers between the computer program and the computation devices is manual though. And the
user has to put transfer orders in the command queues.

Manipulating memory through the abstract layer is achieved by representing memory blocks by buffer
objects. One should dig further the comprehension of the graphic cards memory models in order to
optimize the configuration of these buffers, but this is not the purpose of this tutorial.


creating a command queue
------------------------

For the same reasons that motivated the creation of a memory abstraction layer, it is not possible
to write a program that gets executed by the computation device directly. OpenCL uses a command
queuing mechanism instead.

A queue holds orders for the operations to be executed:
    - data load
    - program execution
    - data retrival

At this point, you might still wonder how parallelization happens in the OpenCL representation of a
computation. It is achieved through the use of the command queues, and more specifically the
program execution order.

Let's use the typical example of unrolling a loop in order to make a parallelized computation.
If we take the following sequential pseudo-code example :

for i in range 1 to 10
    compute f(i)
end

OpenCL offers a range based parallelization. The OpenCL program will look similar to this
pseudo-code:

create R a serie of values in range 1 to 10
equeue computation of f(x) for each x element of R
wait for the end of computation event
retrieve results

At line 2, the computation device will start independant threads each evaluating f(x) with a
different x parameter. 

Please, bare in mind that OpenCL specification imposes no constraint as of the order in wich threads
are started. Thus writing in shared memory requires specific locking and synchronisation systems.

OpenCL parallelization is always range-based, so there is a range object. It is possible to specify
up to 3 ranges of values for a single computation wich is usefull for algorithms running on a
discrete space problem.


- allocating memory on the device
- loading the program that will be executed on each value of the dataset
- creating a command queue
- enqueuing data load
- enqueuing program execution
