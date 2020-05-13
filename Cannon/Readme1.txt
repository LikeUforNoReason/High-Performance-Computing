CPSC High-Performance Computing for Power System Modeling and Simulation
     
Final Project: Parallel implementation of Cannon’s algorithm
Author: Haotian Deng Biyang Fu
Date: Apr 2020 Spring 2020

Project introductions:
In this project, We parallelize Cannon's algorithm. This program can generate two random matrices  according to the parameters which you input, then the program calculates the multiplication of two matrices, finally, the program display the results and elapsed time on the screen.

Usage steps:
1.Build the code through makefile, Dependencies are openmp 1.81, gcc 4.8.1.
 instruction:   make
2. Run program
  instruction:
   ./cannon parameter1 parameter2 parameter3
    parameter1: dimensions of block matrix, used to divide matric
    parameter2: Generated matrix dimensions, used to generate random matrix
    parameter3: number of threads, used to parallel
    For example: ./cannon 2 6 4
     2x2 block matrix; 6x6 random matrix; 4 threads


