# <center>Week 8: Caches</center>

## Addition

With the static schedule, a smaller chunksize of 10 seemed ideal in general (given the sizes of matrices we computed), but there seemed to be a point where a larger larger chunk size was optimal, providing the best speedup on 4000 and 8000 size matrices.

It seems that enforcing a chunksize on an array close to that chunksize just means some threads aren't working when they could be, leading to this dropoff of efficiency with smaller sized tasks.

With the dynamic schedule the results were the inverse, smaller chunk sizes preformed better accross the board. This puzzles me as I would assume that larger chunksize would generally improve performance with larger input size due to the fewer necessary communication routines a system would have to preform with larger chunks.

## Transposition

With transposition I found a major differnce in the effect of chunk sizes between the two scheduling methods.

On matrices greater than 1000 by 1000 the static schedule produced much higher speedup values when given a larger chunk size. Likewise, the dynamic schedule produced similar results, and although the highest value was achieved using the static schedule, it was much more stable: that is it's speedup didn't fall off as the input value increased past 4000.

Detailed timing data for just my tests can be found on a sepparate [sheet](https://docs.google.com/spreadsheets/d/1Gtb4QWdxb_lHFBFZLiGZFeQ0lcC6ua-aBByfodmqR2c/edit?usp=sharing).

[Go Home](/../../jonesh-csc333-s21)
