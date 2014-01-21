The overall process as defined in OpenCL
========================================

The OpenCL specification divides the computation into several parts.
We will present each of them below, with the naming conventions.

choosing a platform
-------------------

The specification is implemented into a library, that support multiple devices.
The first step is to choose which platform implementation will be used if several ones are available.
Usually, a platform is chosen for the devices with wich we want to work.

starting a context
------------------

A context has several roles, it manages memory buffers, devices and command queues.
It is the root object during the whole computation process. It handles devices, memory buffers and command queues. The queues hold the order of the operations to be executed : data load, program execution, data retrival.
The context is usually created from a list of devices obtained from the platform selected during the previous step.

allocating memory
-----------------

The memory model of OpenCL is more complicated than in a stadard computer. For example, data can be split or shared implicitly between devices. However, the transfer of data between the computer program to the computation devices is manual. This is done through Buffer objects. Selecting the proper paramters requires a good comprehension of both the OpenCL memory model and of the device characteristics for a highly optimized execution.

creating a command queue
------------------------

- allocating memory on the device
- loading the program that will be executed on each value of the dataset
- creating a command queue
- enqueuing data load
- enqueuing program execution

