# <center>Week 2 Assignments</center>

# Tasks

- [ ] Array Sum
- [ ] Vector Dot
- [ ] Vector Add

# Observations and Measures

## Array Sum

My algorithm for summation is as follows:
  1. Calculate a scalar (the zize of each chunk) and remainder (the odd one's out)
  2. Assign starting values based on each processor's rank (if root process, skip to 4)
  3. Make lower index chunks (a `remainder` amount of them) process an additional value and adjust the start and end of remaining chunks to after already assigned ones
  4. Preform local addition from `my_start` to `my_end` in the array (if non-root, send local sum via `MPI_Send`)
  5. If root task, sum data recieved via `MPI_Recv` and exit


## Vector Dot

## Vector Add

# Feedback from John

## Array Sum
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysis
- Notes:

## Vector Dot
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysis
- Notes:

##  Vector Add
- [] Code Exists
- [] Code Compiles
- [] Code Runs
- [] Writeup and Analysis
- Notes:
