An Implmentation of the C++ Threads Library

Written by Stephen Opet III, https://github.com/stephen-opet

Published May 24, 2020 [in SARS-COV-2 quarentine :) ]


At its core, this program tests all numbers in an interval & tallies primes

Three implentations execute this operation across 1,2,and 4 threads

Runtime for each implementation is measured with the chrono library

so long as the system has as many cores as threads, 

performance should increase


Designed and tested on UbuntuLinux w/ g++ compiler

FOR THREADS LIB - compile with pthread link:

	g++ -pthread threads.cpp