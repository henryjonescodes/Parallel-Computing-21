# <center>Week 3 Assignments</center>

# Tasks

- [X] Normalize Vector
- [ ] Parallelized 1-D Automaton

# Observations and Measures

## Normalize Vector

For this section I went fairly step by step through the provided guide. Later on, for testing, I implemented a sepparate magnitude calculation function but choose to keep the code more or less duplicated inside the main function because `parallel_magnitude` opperates on a vector of doubles instead of ints.

For time trials I put the same code against itself normalizing 10,000,000 value vectors on one and four processes respectively (my mac has but 4 cores).


One Process | #1 | #2 | #3 | #4 | #5
--- | --- | --- | --- |--- |---
Time(s) | 0.279833 | 0.286686 | 0.273691 | 0.268294 | 0.281156

Average (mean): 0.277932 seconds

Four Processes | #1 | #2 | #3 | #4 | #5
--- | --- | --- | --- |--- |---
Time(s) | 0.181450 | 0.182311 | 0.187043 | 0.185031 | 0.190632

Average (mean): 0.1852934

Average Speedup: 1.4999562

## Parallelized 1-D Automaton

Progress (April 19th): This is a frustrating one. I have the processes correctly choosing their reciving and fetching nodes but I'm currently trying to get `MPI_sendRecv()` for me without causing segmentation faults.
