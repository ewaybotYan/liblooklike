Tree evaluation {#tree_evaluation}
===============

The tree evaluation is performed recursively and automates the following tasks:
- memory allocation on the expressions
- temporary memory allocation for the algorithms
- algorithms executions
- memory release

Evaluation is performed by @ref Algorithm::evaluateTree in @ref Algorithm and
works recursively.
This means if a root algorithm has dependencies that require computation, it
will start evaluation on the parent algorithm of the dependencies which in turn
will perform the necessary tasks to compute their resulting expressions.

This library does not assume synchronous behaviour during the execution. That
is to say that @ref Algorithm::evaluateTree will return as soon as computation
of the algorithm has been *enqueued*. If synchronisation is to be achieved, for
example if a dependency of an algorithm needs to be completely computed before
performing any other computation, then the program must use
@ref waitEndOfEvaluation or any event system provided by implementations of
@ref Algorithm.

@ref Algorithm::evaluateTree follows the following general steps:

@startuml{evaluateTree.png}

start

if (is enqueued?) then (yes)
    stop
else (no)
    while (more dependencies?)
        :enqueue dependency;
    endwhile
endif
:allocate memory for
result expressions;
:allocate temporary memory;
:enqueue execution;
:update status;

stop

@enduml
