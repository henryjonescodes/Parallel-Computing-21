# <center>Week 4: Advanced MPI</center>

# Introduction

This week we learned advanced MPI collective communication, most importantly `MPI_Reduce`, one of the most important concepts in parallel programming.  We'll start by doing some in-class exercises, then move on to a fun application of the prefix sum algorithm.


# In Class Thursday

## Vector Normalization round 2

* Start by copying your vector normalization code into this directory
* update the code to use `MPI_Allreduce` with the `MPI_Sum` operator to get the sum of squares shared with *all* tasks, instead of having the root node receive, accumulate, and then send the sum of squares.
* time your code across several tasks and vector sizes.  Make a note in your `Writeup.md`

## Vector Mean

* Similarly, create an `MPI_Vector_Mean.c` and use the collective communication tools to calculate the mean of a vector.
* Each task should create its *own* local array with random values, and then calculate the local sum.
* Task 0 should then use `MPI_Reduce` to accumulate the local sums.
* time your code across several tasks and vector sizes.  Make a note in your `Writeup.md`

## Vector STD

Now we'll implement standard deviation using the formula and method discussed in lecture

* Next create an `MPI_Vector_STD.c` Using the standard deviation formula.
* Each task should create its own random vector and calculate its local sum
* Each task should use `MPI_Allreduce` to obtain the global sum, and use that to calculate the global mean
* Each task should calculate its local sum of squared differences (squared distance between the value at every index and the global mean)
* Task 0 should then Reduce these local sum squared differences to calculate the standard deviation.


## Submit:

* add/commit/push your solutions above
* Use tables in your `Writeup.md` to provide timing data that is nicely formatted

# Line of Sight Algorithm (1-D)

In this project you'll implement the prefix-max algorithm discussed in lecture in order to calculate line of site on a 1-D map.   There are several steps.

## Step 1: generate a map

We'll use an array of integers to represent altitudes in meters.   To parallelize this algorithm, we'll have each task be responsible for a separate contiguous block of the terrain (i.e. block-distributed).   I've gone ahead and written a function called `MakeTerrain` that makes a terrain by iterating through a local terrain, picking a small altitude change (delta) between +2 and -2, and adding that to the prior value of the terrain (essentially we're doing a prefix sum over the deltas).  To make things interesting, the root node will start its altitude not at zero, but at 100 (meters).


However, we'll need to "connect" these terrains together so that they're contiguous -- right now the starting height at each non-root task is small (+/- 2), but instead we need to increase its local height by the last height of the prior task (that is to say, if task 0's terrain ends at altitude 105, and Task 1's altitude begins at 2, then we want to add 105 to every value in Terrain 1.  

Specifically we need to calculate the node-wise exclusive prefix sum of the last element of every node's terrain, and add that value to every altitude in current task.   In other words:

* Task 0's altitude remains unchanged
* Task 1 altitudes += Task 0's last altitude
* Taks 2 altitudes += (Task 0's last + Task 1's last)
* Task 3 altitudes += (Task 0's last + Task 1's last + Task 2's)
* Task N's altitudes += prefifix sum of Tasks (0..N-1)

You can do this with a single call to `MPI_Exscan()` to get each node's exclusive prefix sum.  I've left a comment in the code where you need to add this.


## Step 2: Calculating and correcting vertical angles

The *vertical angle* from a viewing point `(x,y)` and a target `(x',y')` is defined as `arctan((y' - y)/(x' - x))`.  Recall that the line-of-sight algorithm states: *a point is visible only if no prior point has a larger vertical angle that it*.

 I've written a function `CalculateAngles` that calculates the vertical angle of every point in the local terrain.    This function also calculates the *maximum* vertical angle (prefix max scan) at every index.  

 However, this isn't quite correct, because every Task isn't yet aware of what the prior Task's maximum vertical angle is.  Once again, prefix scan, this time prefix *max* scan will save us.

 Where I've written "Step 2" in the comments, use `MPI_Exscan` once again, this time calculate the max scan of the last value in every Task's `maxangles` array.


I've then written a `FixAnglesAndFindVisible` function which, if your code is correct, will properly generate a local array of `char`, with a 0 if the point is not viewable, and a 1 if it is.

## Step 3: Cleaning up, printing and timing.

I've written code that gathers and prints the visibility array.  Write similar code that gathers a global altitude array and prints it out.  

Format your output so that it can be easily cut/paste into the colab notebook linked to on the nexus site.  

Take timing data looking for speedup across a number of world sizes and tasks.  

## Hand in:

* Your `MPI_LOS.c`
* A writeup describing your contributions to the code as well as nicely formatted timing data *and analysis*.
* You should be able to include images produced by the colab notebook into your writeup (you may add these images to your repo.)
