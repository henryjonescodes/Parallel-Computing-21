# <center>Week 2 Assignments</center>

# Tasks

- [X] Array Sum
- [X] Vector Dot
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

For vector-dot I mostly followed from the given plain-english version on our repo. Some changes and notable tidbits include:
- Initializing vectors of the same name in root and non-root processes to allow for them to be referenced identically.
- Re-use of code from `vectors.c` to randomize and fill vectors (as well as grab command line args).

As per our discussions on Slack I spent a lot of time with `MPI_Send()` before finally cracking it. After some hands on though, I think I'm pretty set to send and recieve with MPI.

### Speedup

My first test used random vectors with `SIZE = 100` on both my `vectors.c` as well as my `MPI_vector_dot.c` files. Although my result here feels like i've done something wrong with timing placement, it showed significant speedup after averaging 5 trials of each and using the basic `Tserial/Tparallel` formula.

- Serial Solution = .00180ms
- Paralel Solution = .0003876ms
- Speedup = 4.6439

My other test used vectors with `SIZE = 100000` again on both programs. With larger vectors the speedup becomes much more interesting. More specifically, it's surprising how fast the serial algorithm slows down when given larger and larger vectors to process while the paralell solution slows down much more gradually.

- Serial Solution = .3782ms
- Parallel Solution = .0123714ms
- Speedup = 30.5705

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
