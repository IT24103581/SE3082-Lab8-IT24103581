# Lab Sheet 6 – MPI

**BSc (Hons) in Information Technology – Year 4**
**SE4060 – Parallel Computing**
**Semester 2, 2021**

## Objectives

- Write MPI programs

---

## Exercise 0

Please complete the in-class activities we did in the MPI lecture given at
https://github.com/SLIITFacultyOfComputing/SE4060-MPI-Part1

## Exercise 1

- Create a GitHub repo with your student id. If you already have one you can use it.
- Login to your GitHub account.
- You have to do the exercises given in the repository and commit and push your solutions into it.

## Exercise 2

Parallelize adding up numbers 1 to 10,000,000 using multiple nodes using MPI.

## Exercise 3

Parallelize the computation of Pi using the "Monte Carlo method" (see sample C code in the following link) using MPI.
http://www.dartmouth.edu/~rc/classes/soft_dev/C_simple_ex.html

Find the calculation for 10,000,000 times.

## Exercise 4

For both Exercise 1 and Exercise 2:

1. Plot a graph that shows Time vs Number of Processors.
2. Plot a graph that shows Speedup.

## Exercise 5

1. Write a simple program where the source and the destination doesn't match. You can take either `message1.cc` or `message2.cc` and modify the program. What happens in this situation?
2. Rewrite `message2.cc` using Buffered Send (BSend). Ensure that you do not replace the variables that you are using to send the data.

## Exercise 6

Rewrite Exercise 3 by changing the receiving Tag to `MPI_ANY_SOURCE`, redo Exercise 3 and compare with the original results.

## Exercise 7

Rewrite Exercise 6 by using Buffered Send (BSend).
