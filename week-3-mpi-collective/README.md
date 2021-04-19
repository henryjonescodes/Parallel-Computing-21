# <center> Week 2 Assignments</center>


# Hands-On: Vector Normalization



## Setup: 

* pull the class repo
* copy `week-3-mpi-collective` into your own repo
* add/commit/push the contents

## Description 

Following a lecture on collective communication protocols and the vector normalization task, implement parallelized vector norm.

Let's first operate under the assumption that the size of the array is evenly divisible by the number of tasks (pick an even number).  

Recall that vector normalization is $\frac{\hat{u}}{||u||}$

Recall the algorithm is as follows:

1. Generate vector on root (make sure N%p == 0)
2. scatter vector  to all nodes
3. all nodes calculate local sum-of-squares
4. all nodes send local sum back to root, root receives
5. root accumulates local sums, takes square root
6. root broadcasts sqrt to nodes
7. all nodes normalize their local vector
8. all nodes gather back to root node

In a file called `MPI_Normalize_Vector.c`, implement the normalization function.  You may use your Week 2 vector programs as starter code.

* you should have separate arrays for the original and result vector
* you can re-use the same array for `local-sum` and `local-norm`

## Tests:

 you'll know it works if the magnitude of the normalized vector is 1.  The magnitude is calculated by taking the square root of the sum of squares of the vector.  

 Write a parallelized function to calculate the magnitude of a vector.

 ## Analysis:

* calculate speedup for a range of vector sizes and tasks.  Use MPI timing code.

 ## Submit:
 *  add/commit/push your code and a `Writeup.md`

 # Challenge: non-even slices

 Learn about `MPI_Scatterv`, and implement a version of vector normalization that works even when $N%P != 0$
 
