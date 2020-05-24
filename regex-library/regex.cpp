/*
	An Implmentation of the C++ Regular Expresstion Library
	Written by Stephen Opet III, https://github.com/stephen-opet
	Published May 24, 2020 [in SARS-COV-2 quarentine :) ]

	This program reads a file in the working directory (named regex.txt here),
	parses the file for valid C++ class identifiers,
	and prints results to the screen
	Implementation shown here can handle up to 2 inherited classes & a multiline class declaration

	Designed and tested on UbuntuLinux w/ g++ compiler
*/
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
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

int main(){
	//////////////////////////////////////////////////////////////////////////////   Pretty Header  ///////////////////
	textcolor('g');
	cout << "\n\n_________________________________________________________________________________\n\n";
	cout << "An Implementation of the C++ Regular Expresstion Library\n";
	cout << "Written by Stephen Opet III\n";
	cout << "https://github.com/stephen-opet\n\n\n";
	textcolor('w');


	//////////////////////////////////////////////////////////////////////////////   Create RegEx String  ///////////////////

	//This regex string parses an input for all valid C++ class identifiers
	//Operation is as follows:
		//1) Find at least one instance of 'class', followed by whitespace
		//2) Search for at least one succeeding legal variable; store as group 1
			//3) Search for optional inheritance; whitespace, followed by colon operator, followed by whitespace
			//4) Search for optional access modifier (public, private,protected) with whitespace
			//5) Search for legal identifier for first inherited class; save as group 2
				//6) Search for optional second inherited class; whitespace, followed by a comma, followed by whitespace
				//7) Search for optional access modifier (public, private,protected) with whitespace
				//8) Search for legal identifier for second inherited class; store as group 3
		//9) Search for optional whitespace, tab, or linebreak following class declaration
	string cpp_classes = R"(class[ \t]+([A-Za-z_][A-Za-z0-9_]*)[ \t]*[:]*[ \t]*(?:public[ \t]*|private[ \t]*|protected[ \t]*)*([A-Za-z_]*[A-Za-z0-9_]*)[ \t]*,*[ \t]*(?:public[ \t]*|private[ \t]*|protected[ \t]*)*([A-Za-z_]*[A-Za-z0-9_]*)[ \t\r\n]*)";
	regex r(cpp_classes);

	//////////////////////////////////////////////////////////////////////////////   Read In C++ File  ///////////////////

	ifstream f("regex.txt");
	string file_data = "";
	file_data.assign( istreambuf_iterator<char>(f) , istreambuf_iterator<char>() );
	if(file_data == ""){
		textcolor('r');
		cout << "\tError accessing data file.\n";
		textcolor('w');
		cout << "\tMake sure regex.txt is in the working directory\n";
		return endProgram(0);
	}


	//////////////////////////////////////////////////////////////////////////////   Run RegEx Search  ///////////////////

	smatch m;
	bool first = true;
	bool parent = true;

	cout << "Are there any valid C++ Classes in your textfile?\n\n";

	while(regex_search(file_data,m,r)){
		string classes[3] = {"","",""};
		for(auto x : m){
			if(first)
				first = false;
			else{
				if(parent){
					cout << "\tClass Found:  " << x << '\n';
					parent = false;
				}
				else{
					if(x != "")
						cout << "\t\tInherited Class Found:  " << x << '\n';
				}
			}
		}
		first = parent = true;
		cout << '\n';
		file_data = m.suffix().str();
	}

	return endProgram(0);
}