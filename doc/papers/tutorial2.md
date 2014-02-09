% Tutorial 2 : operation planning and synchronisation in OpenCL
% Nicolas Granger
% December 18, 2013



Note to readers
===============

This tutorial follows an introduction to parallized environments, more
specifically the one defined by OpenCL. OpenCL objects names will be used, but
most concepts are shared across the different parallelized environments.

Introduction
============

When we concieve a sequential algorithm, all functions are blocking the
execution flow until the instructions are completed.
In the case of concurrent execution, a function is applied
simultaneously to an array of values. However, the execution of single instances
of the function is not synced between each process. One function instance 
might finish its work before another one.

If you consider a sequence of operations such as the evaluation of a
mathematical expression, it appears that some instructions cannot be started
until the result of another is available.

  example:
  a * ( b + c )
  ( b + c ) must be computed before the product operation can start

These are two different levels of synchronization.

Inter-kernels synchronization
============================

The synchronization between operations inside the same execution queue
