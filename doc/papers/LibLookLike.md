Parallelization and OpenCL : an example with facial recognition 
===============================================================


Parallelization and OpenCL 
==========================


A short presentation of parallelization 
=======================================

The word parallelization is becoming very common in Computer Science, and has 
spread outside laboratories. Intense computation tasks are widely used in 
various domains outsides science : games, picture editing, pattern 
recognition...  

To simplify, we can consider two of the fields of study in computer science: 
- the improvement of computer performances 
- the design of more optimized algorithms.

Parallelization bridges both of these orientations. It addresses the limitation 
of the standard computers by multiplying the computation devices inside the 
machine, but requires specifically designed algorithms, that can split work 
among the devices instead of queuing tasks on a single one.  Thus if you 
replicate six times your computation devices and split work equally between 
each one, you can expect a performance improvement by a factor up to six. 

Figure [] shows the difference between sequential, and parallel execution. 

Further in this series of tutorials, we shall call computation device the 
object that takes data input, executes a computation on it and returns the 
result of the algorithm. Parallelization will happen inside this computation 
device.

If the problem of parallelization is indeed to create a compatible algorithm, 
the existence of physical architectures providing the assets to run the 
programs is in fact quite recent. The first solution was to build a pool of 
computers inside a network, and develop a software layer on top to synchronize 
tasks and memory operations. 

Graphic cards have lately come into the field of computation devices.They 
actually are all-in-one parallel computation devices boasting several thousands 
of microprocessors optimised for float computation and several types of memory.  
They provide higher computation efficiency than a classical CPU which is 
designed for more complex tasks. 


Parallelization with OpenCL
===========================

The diversity of graphic cards and more generally of devices with 
parallelization abilities is wide.  As a consequence, a uniform programming 
interface had to be designed in order to provide a common standard for parallel 
computation. This enables developers to provide a single code that can be run 
on a wide range of devices. 

The solution we shall explore here is called OpenCL, but this is not the only 
one (CUDA (tm) is another example). OpenCL objects names will be used, but most 
concepts are shared across the different parallelized environments.

OpenCL is a specification. This specification  was not published by a graphic 
card manufacturer, but rather by the Chronos Group who designed it and has had 
a long relationship with the graphic cards market.  

Being a specification, it only defines a set of functions with a description of 
the expected behaviour.  The purpose of these functions is to handle the 
operations needed to execute a computation on a device. Thus you can find 
functions ranging from devices listing to data and programs loading.  As of 
now, the specification has been implemented by the main desktop computer part 
makers, but also for high end embedded devices such as cellphones.  The 
libraries however do not always implement all the functionalities of the 
specification.


Implementation of parallelization : the overall process as defined in OpenCL 
============================================================================

The OpenCL specification divides the computation process into several parts.  
We shall present each of them below, and introduce the naming convention used 
in the specification. 

Choosing a platform
-------------------

The specification is implemented into a library, that is to say a piece of 
computer program running on both the computer and the computation device. The 
first step is to choose which platform implementation will be used if several 
ones are available.  Since most computation devices manufacturer provide a 
library for their own devices, you usually end up choosing the library that 
matches the devices on which we want to work.  

Ex: If you wish to work with an AMD graphic card, you will want to use AMD\'s 
OpenCL implementation.

Note that a platform can handle multiple devices at the time, provided that all 
of them are supported by the implementation. Thus you can imagine a computation 
device that is actually an aggregation of several identical graphic cards. 

Starting a context
------------------

A context has several roles, it manages memory buffers, devices and command 
queues. The context in a Object oriented programming point of view is what 
matches best what we have called computation device so far. It is the root 
object during the whole computation process.

The context is usually created from a list of devices listed by the platform 
selected during the previous step.

Allocating memory
-----------------

The memory model of OpenCL is more complicated than in a standard computer. For 
example, data can be split or shared implicitly between devices. More generally 
there is an abstraction layer between the real memory management inside the 
computation device and the OpenCL interface. The purpose of this complex system 
is to make parallelization possible in the device without low level hardware  
considerations, as everything is automatically handled by the OpenCL 
implementation.  Data transfers between the computer program and the 
computation devices is manual though. And the user has to put transfer orders 
in the command queues.

Manipulating memory through the abstract layer is achieved by representing 
memory blocks by buffer objects. One should dig further the comprehension of 
the graphic cards memory models in order to optimize the configuration of these 
buffers, but this is not the purpose of this document.


Creating a command queue
------------------------

For the same reasons that motivated the creation of a memory abstraction layer, 
it is not possible to write a program that gets executed by the computation 
device directly. OpenCL uses a command queuing mechanism instead. 

A queue holds orders for the operations to be executed: 
    - data load
    - program execution
    - data retrieval

At this point, you might still wonder how parallelization happens in the OpenCL 
representation of a computation. It is achieved through the use of the command 
queues, and more specifically the program execution order.

Let\'s use the typical example of unrolling a loop in order to make a 
parallelized computation.  If we take the following sequential pseudo-code 
example :

    for i in range 1 to 10 
        compute f(i)
    end 

OpenCL offers a range based parallelization. The OpenCL program will look 
similar to this pseudo-code:

    create R a series of values in range 1 to 10 
    enqueue computation of f(x) for each x element of R 
    wait for the end of computation event 
    retrieve results. 

At line 2, the computation device will start independent threads each 
evaluating f(x) with a different x parameter.

Please, bare in mind that OpenCL specification imposes no constraint as of the 
order in which threads are started. Thus writing in shared memory requires 
specific locking and synchronisation systems.

OpenCL parallelization is always range-based, so there is a range object. It is 
possible to specify up to 3 ranges of values for a single computation which is 
useful for algorithms running on a discrete space problem. 

Enqueuing data load and program execution
-----------------------------------------

Since the kernels are executed without specific order, OpenCL offers a 
mechanism to handle the order end the execution of tasks. Every OpenCL 
operation is stored in a command queue and each event has an associated end 
event.

Commands in the queue are executed as soon as possible with respect to other 
events if dependencies are specified between commands.


OpenCL in LibLookLike
=====================

This project tries to simplify the integration of OpenCL and hence offers a
wrapper around the OpenCL library in order to simplify the work of creating
OpenCL accelerated algorithm.

A platform is automatically chosen with various sanity checks. Loading kernels 
and handling dependencies is done transparently thanks to a framework that 
standardizes algorithms and mathematical objects writing.


Computation tree : expressions, operators and sets 
==================================================

When we conceive a sequential algorithm, all functions are blocking the 
execution flow until the instructions are completed. In the case of concurrent 
execution, a function is applied simultaneously to an array of values. However, 
the execution of single instances of the function is not synced between each 
process. One function instance  might finish its work before another one. 

If you consider a sequence of operations such as the evaluation of a 
mathematical expression, it appears that some instructions cannot be started 
until the result of another is available. 

  Example:
  a * ( b + c )
  ( b + c ) must be computed before the product operation can start

In LibLookLike, the operations are represented as a tree : the leaves of the 
tree are expressions and the nodes are operators.  Each expression is of a 
certain type, or set : it can be a scalar value, a matrix, a vector, etc... 

The tree evaluation is performed recursively and automates the following tasks: 
- memory allocation for the leaves concerned by the same operator
- memory allocation for the result(s) and temporary memory if needed
- execution of the operation.

When an operation is executed, the node representing the operation has a new 
leaf storing the expression corresponding to the result of the operation.

The evaluation of a whole computation tree is performed by the operators and 
works recursively.  This means if a root operation has dependencies which 
require computation, it will start evaluation on the root operation of the 
dependencies which in turn will perform the necessary tasks to compute their 
resulting expressions.


Expressions and operators : Expression and Algorithm classes 
============================================================

Expression
----------

Expressions in LibLookLike are represented by the class Expression, which is an 
abstract class defining a set of common characteristics.  When evaluating an 
expression, the operation attached to the expression will start its execution.  
If no operation is attached to an expression, it means that the expression does 
not need to be computed. 

Algorithm
---------

Operations in LibLookLike are represented by the class Algorithm, which is a 
standardized interface on the operations.  The Algorithm class automates the 
memory allocations, dependencies and execution for the computation of 
expressions. 

An algorithm takes expressions in and yields results which are expressions 
themselves. The input is also considered as dependencies of the algorithm.

When executing an operation, if the input values are not yet evaluated, it will 
automatically start their evaluation. Then the actual computation performed by 
the operation will start.

Interactions between Expression and Algorithm 
---------------------------------------------

The method getResult() (or an equivalent function depending on the algorithm) 
applied on a algorithm returns an expression, not yet computed, but related to 
this algorithm. Then, with such an expression as input, it is possible to 
create other operations.  Calling getResult() on these operations will return 
expressions whose actual value is still unknown. A call to the method 
evaluate() will fill the expression with its actual value, after all the 
dependencies of this expression have been evaluated recursively.


Facial recognition and parallelization in LibLookLike 
=====================================================


Facial recognition by principal components analysis in a nutshell 
=================================================================

We assume that readers are familiar with data analysis by PCA (principal 
component analysis).  We will only present our choices to realise a PCA on 
faces.

We have a database of 200 images, each image representing one person. Each 
image is centred, and manually modified, so that the eyes of each person are 
about at the same place than those of any other person. 

The photographs have a size of 360*260 pixels.  To recognize a person, we have 
to project one\'s image, this is to say the vector of pixels representing the 
person, on the eigenfaces, this is to say, the vectors of pixels representing 
the eigenfaces. So we have to project a person in the cloud of the pixels. But 
such a projection implies a correlation matrix of size (360*260)^2. So we work 
in the cloud of the persons (the correlation matrix is thus of size 200*200) 
and then we go from the cloud of persons to the cloud of pixels by multiplying 
left by the matrix of the persons. 

# normalization ???

Parallelized algorithms in LibLookLike
======================================

In LibLookLike, four major operations are parallelized: 
- the multiplication of matrices
- the multiplication between a transposed matrix and a matrix
- the multiplication of a transposed matrix and itself
- the normalization of a matrix.

These parallelized algorithms are implemented, as usual, in Algorithm classes, 
which use OpenCL kernels to distribute computations on CPUs or GPUs.  We will 
present here three of the four operations above.

Multiplication of matrices
--------------------------

The result of the multiplication of two matrices is computed by executing the 
computation of each element of this matrix in parallel, this is to say, the 
vectorial products between each row of A and each column of B in the product 
A*B are executed in parallel. 

Multiplication between a transposed matrix and a matrix 
-------------------------------------------------------

The multiplication of a transposed matrix and another matrix is computed in the 
same way than the multiplication of two matrices, except that the vectorial 
products executed in parallel are between each column of A and each column of B 
in the product t(A)*B, provided that the dimensions of the matrices allow to 
compute such a product. 

Normalization of a matrix
-------------------------

The normalisation of a matrix can be executed in respect to the rows, or in 
respect to the columns. Both are possible in LibLookLike.  We will present here 
the normalisation on the columns. Both normalisations follow the same 
principle, anyway.  To normalise a matrix on the columns,  the program executes 
the computations in parallel for each column : compute the average and 
variance, and then the new terms of the matrix X as themselves minus the 
average over the variance. Note that the computation of the variance in the column
normalisation in LibLookLike is improved with the Kahan summation algorithm to avoid cancellation.

Later, when computing the matrix t(X)*X, the result will be the matrix of 
covariance. So, at this stage, we shall divide the result of the computation 
of t(X)*X by n, in order to obtain the matrix of correlation.
