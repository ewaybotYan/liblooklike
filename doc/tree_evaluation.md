Tree evaluation {#tree_evaluation}
===============

The tree evaluation is performed recursively and automates the following tasks:
- memory allocation on the expressions
- temporary memory allocation for the algorithms
- algorithms executions

Evaluation is performed by @ref Algorithms and works recursively. This means if
a root algorithm has dependencies that require computation, it will start
evaluation on the parent algorithm of the dependencies which in turn will
perform the necessary tasks to compute their resulting expressions.

This project does not assume synchronous behaviour in the execution. That is to
say that @evaluateTree will return as soon as computation of the algorithm has
been launched. If synchronisation is to be achieved manually, for example inside
the algorithms, it can still rely on event system provided by @ref Algorithm
implementations or by using @ref waitEndOfEvaluation .

@ref evaluateTree follows the following general steps:

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
