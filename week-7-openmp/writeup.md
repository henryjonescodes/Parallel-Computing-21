# <center>Week 7: OpenMP</center>
### <center>Henry Jones</center>

For both tasks I ran into problems running on the virtualized threads on my personal machine. My model processor has 4 real threads with access to 8 total threads via hyperthreading, processor info can be found [here](https://everymac.com/systems/apple/macbook_pro/specs/macbook-pro-core-i7-2.6-15-mid-2012-retina-display-specs.html).

# Vector Normalization with OpenMP

For normalization, I timed only the single call to my normalization function with 1, 4, and 8 threads, and using a vector size of 100,000,000 indices.

| OMP_Vectors | #1       | #2       | #3       | #4       | #5       | Average (Mean) |
|-------------|----------|----------|----------|----------|----------|----------------|
| 1 Thread    | 1.568661 | 1.376493 | 1.367147 | 1.371477 | 1.374542 | 1.411664       |
| 4 Threads   | 0.789219 | 0.775255 | 0.774533 | 0.724535 | 0.723951 | 0.7574986      |
| 8 Threads   | 1.144463 | 1.275404 | 1.216384 | 1.208396 | 1.115971 | 1.1921236      |

Speedup (4 cores): 1.86358628254

Speedup (8 cores): 1.18415909223

Although legitimate speedup was achieved by utilizing all 4 real threads my laptop has to offer, its clear that the returns from leaning on the additional 4 virtualized threads is negligable.

# Multithreaded pi Estimation

For pi, I again only timed my pi estimation function, running on 1, 4, and 8 threads like before. This time I wanted a larger input to push the parallelized time into the seconds and went with N = 1,000,000,000 which also just feels cool in my oppinion.

| OMP_pi    | #1        | #2        | #3        | #4        | #5        | Average (Mean) |
|-----------|-----------|-----------|-----------|-----------|-----------|----------------|
| 1 Thread  | 13.082241 | 12.386034 | 12.709434 | 12.198256 | 13.415441 | 12.7582812     |
| 4 Threads | 3.775067  | 3.795935  | 3.556785  | 3.603549  | 3.840654  | 3.714398       |
| 8 Threads | 3.418515  | 3.380837  | 3.264555  | 3.401096  | 3.073062  | 3.307613       |

Speedup (4 cores): 3.43481802435

Speedup (8 cores): 3.85724726563

The same diminishing returns are present here, where, despite the massive speedup observed on 4 threads there was little benifit to be had from running on 8 threads.

# Thoughts

As a relative novice to multithreaded programming, I'm not sure if my program can actually take advantage of those virtualized threads. My suspicion is that despite telling it to use 8, openMP might not have access to hyperthreading and is choosing to use the max it has (4) instead.





[Go Home](/../../jonesh-csc333-s21)
