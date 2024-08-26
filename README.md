# mpi_bingo_calc
example mpi program

Calculates the odds of having a coverall bingo given the number of balls of each letter called.

prob = $\prod_{i=1..5}$ $\frac{\binom{15-k_i}{N_i-k_i}} {\binom{15}{N_i}}$

where ${N_i}$ is the number of balls called for each column $i$ and $k_i$ is the number of numbers
on the card for each letter $k_0 = k_1 = k_3 = k_4 = 5$ and $k_2$ = 4 because of the free space.

This example mpi program computes each part of the product as a task list distributed across the process.

Example run:

```
$ mpicc mpibingo.c
$ mpiexec -n 5 ./a.out
Enter number of balls called from each letter
Example: 13 12 10 10 14
14 14 14 14 14
rank=0, col=0, prob=0.666667
rank=1, col=1, prob=0.666667
rank=4, col=4, prob=0.666667
rank=2, col=2, prob=0.733333
rank=3, col=3, prob=0.666667
probability of coverall bingo = 0.144856
```

