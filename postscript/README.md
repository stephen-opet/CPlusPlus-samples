A Demonstration of Postscript Image generation

Written by Stephen Opet III, https://github.com/stephen-opet

Published May 24, 2020 [in SARS-COV-2 quarentine :) ]


This program demonstrates inheritance in C++

Polymorphic functions are used to generate a geometric image

Upon execution, a file containing postscript commands will be produced

postscript runs on most major operating systems via ghostscript like so:

	compile: g++ postscript.cpp

	execute: ./a.out

	run postscript: gs image.ps


Designed and tested on UbuntuLinux w/ g++ compiler