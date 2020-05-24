/*
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
*/
#include <iostream>
#include <cmath>	//fmod - prime function
#include <thread>	//multiple threads - must link compilation (g++ Opet_Stephen_primeThreads.cpp -lpthread)
#include <unistd.h>
using namespace std;

/* We are going to measure the computational cost
	of each method by comparing run times 		
	Reference: https://learntechway.com/simple-timer-in-c-using-chrono-library	*/
#include<chrono>
#include<ctime>

inline void textcolor(char c){
	switch(c) {
		case 'n': {cout << "\33[1;30m";break;} //black
		case 'r': {cout << "\33[1;31m";break;} //red
		case 'g': {cout << "\33[1;32m";break;} //green
		case 'b': {cout << "\33[1;34m";break;} //blue
		case 'y': {cout << "\33[1;33m";break;} //yellow
		case 'w': {cout << "\33[1;37m";break;} //white
		case 'd': {cout << "\33[0m";break;}	   //reset
}	}

int endProgram(int v){	//eloquently abort main() 
	textcolor('r'); cout << "\n\tEnd Program\n\n";
	textcolor('d');	//restore defaults
	return v;
}

bool primeTest(uint64_t n){
	   // Corner cases 
    if (n <= 1)  return false; 
    else if (n <= 3)  return true; 
    else if (n%2 == 0 || n%3 == 0) return false; 
    for (int i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return false; 
  
    return true;
}

void countPrimes(uint32_t a, uint32_t b, uint64_t* primeCount) {

	uint32_t count = 0;

	for(; a <= b; a++){
		if(primeTest(a))
			count++;
	}

	(*primeCount) = count;
}

int main(int argc, char *argv[]){
	//////////////////////////////////////////////////////////////////////////////   Pretty Header
	textcolor('g');
	cout << "\n\n_________________________________________________________________________________\n\n";
	cout << "An Implementation of the C++ Threads Library\n";
	cout << "Written by Stephen Opet III\n";
	cout << "https://github.com/stephen-opet\n\n\n";
	textcolor('w');

	/////////////////////////////////////////////////////////////////////////////	Declare Variables
	const uint32_t n = 10000000;	//krug's default value (100M) takes ~187 seconds on my PC
									//reduce by a factor of 10-100 for mere mortal PCs
	uint64_t count_1thread = 0;
	uint64_t count_2threadA = 0;
	uint64_t count_2threadB = 0;
	uint64_t count_4threadA = 0;
	uint64_t count_4threadB = 0;
	uint64_t count_4threadC = 0;
	uint64_t count_4threadD = 0;


	std::chrono::steady_clock::time_point a = std::chrono::steady_clock::now(); //start timer 
	countPrimes(1,  n, &count_1thread);
	std::chrono::steady_clock::time_point b = std::chrono::steady_clock::now();	//stop timer
	double single_duration_ms = std::chrono::duration_cast<std::chrono::duration<double> >(b-a).count()*1000;


	cout << "\tRange for Prime Numbers: 1 - " << n << "\n\n"; 
	cout << "\tTotal Number of Prime Numbers:  " << count_1thread << '\n';
  	cout << "\tSingle-Thread Duration (ms): " << single_duration_ms << "ms" <<  "\n\n";
	
	// now benchmark with 2 threads
	std::chrono::steady_clock::time_point c = std::chrono::steady_clock::now(); //start timer 
	thread t1( countPrimes, 1,  n/2, &count_2threadA);
	thread t2( countPrimes, (n/2)+1, n, &count_2threadB);
	t1.join();
	t2.join();
	std::chrono::steady_clock::time_point d = std::chrono::steady_clock::now();	//stop timer
	double double_duration_ms = std::chrono::duration_cast<std::chrono::duration<double> >(d-c).count()*1000;

	cout << "\tTotal Number of Prime Numbers: " << count_2threadA + count_2threadB << '\n';
	cout << "\tTwo-Thread Duration (ms):  " << double_duration_ms << "ms" << "\n\n";

	// finally benchmark with 4 threads
	//will only demonstrate improvement for systems running 4+ cores
	std::chrono::steady_clock::time_point e = std::chrono::steady_clock::now(); //start timer 
	thread t41( countPrimes, 1,  n/4, &count_4threadA);
	thread t42( countPrimes, (n/4)+1, n/2, &count_4threadB);
	thread t43( countPrimes, (n/2)+1, 3*n/4, &count_4threadC);
	thread t44( countPrimes, (3*n/4)+1, n, &count_4threadD);
	t41.join();
	t42.join();
	t43.join();
	t44.join();
	std::chrono::steady_clock::time_point f = std::chrono::steady_clock::now();	//stop timer
	double quad_duration_ms = std::chrono::duration_cast<std::chrono::duration<double> >(f-e).count()*1000;

	cout << "\tTotal Number of Prime Numbers: " << count_4threadA + count_4threadB + count_4threadC + count_4threadD << '\n';
	cout << "\tFour-Thread Duration (ms):  " << quad_duration_ms << "ms" << "\n\n";

	//////////////////////////////////////////////  That's all folks	
	return endProgram(0);
}