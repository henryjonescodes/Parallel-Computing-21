# <center>Week 3 Assignments</center>
### <center>Henry Jones</center>

# Tasks

- [X] Normalize Vector
- [X] Parallelized 1-D Automaton

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

Average (mean): 0.1852934 seconds

Average Speedup: 1.4999562

## Parallelized 1-D Automaton

My communication scheme uses blocking `MPI_Send()` and `MPI_Recv()` to pass messages, but staggers the communication based on the processor number, either even or odd. Even processors begin by sending then recieving from the 'right', followed by sending and recieving to the 'left', odd processors do the opposite. This allows the program to run smoothly (given P/ID == 0) despite the blocking functions.

For timing I used a `WORLDSIZE = 260` and `maxiters = 1,000,000` with rule 30 on 1 and 4 processes respectively. I timed most of the main function, from just after command line imput is interpreted up until before the single `printWorld` call near the end of the file.

One Process | #1 | #2 | #3 | #4 | #5
--- | --- | --- | --- |--- |---
Time(s) | 7.270382 | 7.307137 | 7.283688 | 7.252132 | 7.219669

Average (mean): 7.2666016 seconds

Four Processes | #1 | #2 | #3 | #4 | #5
--- | --- | --- | --- |--- |---
Time(s) | 3.121701 | 3.095834 | 3.078397 | 3.077086 | 3.173466

Average (mean): 3.1092968 seconds

Average Speedup: 2.33705627

# Feedback

Normalize: 10/10
- [+] Code Exists
- [+] Code Compiles
- [+] Code Runs
- [+] Writeup and Analysis
- Notes: 

Cell: 29/29
- [+] Code Exists
- [+] Code Compiles
- [+] Code Runs
- [+] Writeup and Analysis
- Notes: 

Challenge: 1/1
- [+] Code Exists
- [+] Code Compiles
- [+] Code Runs
- [+] Writeup and Analysis
- Notes:  pretty flowers!

Total: 40

Overall: A good and generally thorough writeup.  In the future I'd like to see a bit more data, and a bit more insight into *why* speedup is or isn't achieved.