/*
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
*/

//include books as follows:
	//  ./executable book1.txt book2.txt ... 
	//	book files should be space-delineated
	//	include line-delineated dictionary file in working directory as "dict.txt"

#include <iostream>
#include <unordered_map>
#include <fstream> 			//file i/o
using namespace std;

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

int main(int argc, char *argv[]){
	//////////////////////////////////////////////////////////////////////////////   Pretty Header
	textcolor('g');
	cout << "\n\n_________________________________________________________________________________\n\n";
	cout << "An Implementation of the C++ Unordered Map Library\n";
	cout << "Written by Stephen Opet III\n";
	cout << "https://github.com/stephen-opet\n\n\n";
	textcolor('w');

	//////////////////////////////////////////////////////////////////////////////// Variable declarations
	unordered_map<string,uint64_t> dictionary;	//declare dictionary list
	ifstream f("dict.txt");		//open dictionary file
	string line = ""; 			//containter for lines from files
	uint64_t key = 1; 			//iterative key for map

	///////////////////////////////////////////////////////////////////////////////  Build dictionary from dict.txt
	while(true){				
		getline(f, line); 		//Dictionary supplied by ethan is line-delineated
		if(line.length() == 0){break;}	//Abort if we reach an empty line
		else{
			dictionary[line] = key++;	//add key-value pairs to dictionary
			if(f.eof()){break;}			//Abort at end of file
		}
	}
	f.close();	//close the file - allows reuse of "f" for book files 
	cout << "\tYour Dictionary Contains " << key-1 << " words\n\n";

	///////////////////////////////////////////////////////////////////////////////	 Iterate through book files via CMD args
	cout << "\tBook words absent from the dictionary:\n";
	for(int i = 1; i < argc; i++){	//iterate through book file arguments
		ifstream f(argv[i]);		//open current book file
		while(true){				//loop through argument file
			getline(f,line, ' ');	//space-delineated books
			if(line.length()==0){break;}	//Abort if we reach an empty line
			else{
				//GDB reveals dictionary appended escape character \r to values - weird but manageable here
				if(dictionary.find(line) == dictionary.end() && dictionary.find(line + "\r") == dictionary.end())							
					cout << "\t    " << line << "\n";					
			}
			if(f.eof()){break;}	//abort at last value 
		}/* End of book file */	f.close();		
	}
	endProgram(0);
}