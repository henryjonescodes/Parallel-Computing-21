
<center>Henry Jones</center>

# Task List
- [x] Complete Setup (clone repos)
- [x] Assignment 1: Factorization
- [x] Assignment 2: Vectors
- [] Assignment 1: Linked Lists


# Assignment 1

By my understanding, the reason tests take different amounts of time is due to the current state of caches/computer resources. In this case it is probably determined by the other tasks that my computer is working on at each specific moment and how easily it can utilize the remaining resources.

All tests run with the following specs on the provided VirtualBox VM:

* Memory: 4096mb
* Processors: 4
* CPU: Ryzen 7 3700X

## Part 3 Data:

BigNumber: 202151201131415575
* Search 1: 0.089000 ms
* Search 2: 0.100000 ms
* Search 3: 0.102000 ms
* Search 4: 0.093000 ms
* Average: .096 ms

BigNumber: 4185633689926419097
* Search 1: 10.183000 ms
* Search 2: 10.343000 ms
* Search 3: 8.865000 ms
* Search 4: 10.876000 ms
* Average: 10.06675 ms

## Part 4 Data:

BigNumber: 202151201131415575
* Search 1: 0.095000 ms
* Search 2: 0.093000 ms
* Search 3: 0.119000 ms
* Search 4: 0.094000 ms
* Average: .10025 ms

BigNumber: 4185633689926419097
* Search 1: 5.073000 ms
* Search 2: 5.430000 ms
* Search 3: 5.505000 ms
* Search 4: 5.295000 ms
* Average: 5.32575 ms

# Assignment 2

## Part 4:

Increasing the `SIZE` argument beyond the actual vector size begins printing the next locations in memory with both the `print_vector` and `print_vector_p` methods. Since the next locations in memory are other vectors that have already been initialized the program prints out the following vectors. The strange part for me is that there is a zero value between the two vectors, is this some sort of buffer between two variables stored in memory?


## Part 7:

Adding timing revealed that both pointer-based methods `print_vector_p` and `randomize_vector_p` ran consistently faster than their pass-by-value counterparts. The only thing that would cause this seems to be where `print_vector` and `randomize_vector` have to access the actual vector data structure through `vec[index]`. There's likely another step in there that's eliminated by accessing the data in memory directly with the pointer-based versions.

Overall all methods run linearly since all tasks (printing, randomization, addition, and dot product) incorporate single (or parallel) for loops. As such, the methods only started to slow down when I tried extremely large values.