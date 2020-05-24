An Implementation of the C++ Unordered Map Library

Written by Stephen Opet III, https://github.com/stephen-opet

Published May 24, 2020 [in SARS-COV-2 quarentine :) ]


This program demonstrates the following tactics:

	A variable number of command line arguments

	creating an unordered map from lib

	reading data from a file


1) A dictionary is created from the external file, dict.txt

2) space-delineated books as CMD args are read in

3) words found in the books but not in the dictionary are printed to screen


Designed and tested on UbuntuLinux w/ g++ compiler


include books as follows:

	  ./executable book1.txt book2.txt ... 

		book files should be space-delineated

		include line-delineated dictionary file in working directory as "dict.txt"