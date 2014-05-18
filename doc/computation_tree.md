Computation tree    {#computation_tree}
================

This library manipulates mathematical values and performs computation on them.

Everything that has a value, either a known value or the result of a
computation will be called an expression. An expression can be a matrix, a
scalar value...

Any operation that perform computations on expressions will
be standardized in an object that we call algorithm. Additions, multiplications
are typical examples of algorithms.

An algorithm takes one or more expression in input and yields one or more
result expressions.

Expressions and algorithms can be chained into more complex computations: the
result of one algorithm can becom the input of another. If we want to compute
\f$( A + B ) \times C\f$, the objects hierarchy will look like:

@startuml{evaluation_tree.png}
object A
object B
object Sum
object Result1
object C
object Product
object FinalResult

Sum -- A
Sum -- B
Result1 -- Sum
Product -- Result1
Product -- C
FinalResult -- Product
@enduml
