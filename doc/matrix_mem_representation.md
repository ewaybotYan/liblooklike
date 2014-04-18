Matrix representation in memory     {#matrix_mem_representation}
===============================

The values are stored in a 1D array which is the concatenation of the
columns from left to right.

If we replace the elements of a 2x2 matrix with their index in the
buffer, we would have:
    {1, 3}<br/>
    {2, 4}
