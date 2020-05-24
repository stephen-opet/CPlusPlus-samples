/*
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
*/

#define _USE_MATH_DEFINES	//PI
#include <iostream>
#include <fstream>
#include <string>		//string objects
#include <vector>
#include <math.h>		//sin, cos
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

//abstract class for all shapes; contains x and y offsets
class Shape {
	private:
		double xOffset, yOffset;

	public:
	  Shape(double x, double y) : xOffset(x), yOffset(y){} 

	  double getxOffset() const{ //get method for x offset
			return xOffset;
	  }

	  double getyOffset() const{ //get method for y offset
			return yOffset;
	  }

	  virtual void draw(ostream& s) = 0; //OVERWRITE IN CHILDRENS
};

//child of shape; contains rectangular L & W
class Rect : public Shape{
	private:
		double xLength, yLength;

	public:
		//constructor
		Rect(double a, double b, double c, double d) : Shape(a,b),xLength(c),yLength(d) {} 

		//get method for rectagular length
		double getxLength() const{ return xLength; }

		//get method for rectangular width
		double getyLength() const{ return yLength; }

		//overwrite draw method with a rectangular outline
		void draw(ostream& s){
			s << getxOffset() << ' ' << getyOffset() << " moveto\n";
			s << getxOffset() + getxLength() << ' ' << getyOffset() << " lineto\n";
			s << getxOffset() + getxLength() << ' ' << getyOffset()+getyLength() << " lineto\n";
			s << getxOffset() << ' ' << getyOffset()+getyLength() << " lineto\n";
			s << "closepath\nstroke\n";
		}
};

//child of Rect, grandchild of Shape
class FilledRect : public Rect{
	public:
		//constructor
		FilledRect(double a, double b, double c, double d) : Rect(a,b,c,d){} 

		//overwrite draw method with a fill
	  	void draw(ostream& s){
			s << getxOffset() << ' ' << getyOffset() << " moveto\n";
			s << getxOffset() + getxLength() << ' ' << getyOffset() << " lineto\n";
			s << getxOffset() + getxLength() << ' ' << getyOffset()+getyLength() << " lineto\n";
			s << getxOffset() << ' ' << getyOffset()+getyLength() << " lineto\n";
			s << "fill\nstroke\n";
		}
};

//child of shape; contains radius
class Circle : public Shape{
	private:
		double radius;

	public:
		//constructor
		Circle(double a, double b, double c) : Shape(a,b),radius(c) {} 

		//get method for circular radius
		double getRadius() const{ return radius;}

		//overwrite for "draw", circular outline
	  	void draw(ostream& s){ s << getxOffset() << ' ' << getyOffset() << ' ' << getRadius() << " 0 360 arc stroke\n";}
};

//Child of circle, grandchild of shape
class FilledCircle : public Circle{
	public:
		//constructor
		FilledCircle(double a, double b, double c) : Circle(a,b,c){} 

		//overwrite Circle draw with a fill
		void draw(ostream& s){ s << getxOffset() << ' ' << getyOffset() << ' ' << getRadius() << " 0 360 arc fill stroke\n"; }
};

class Line : public Shape{
	private:
		double xEnd, yEnd;

	public:
		//constructor
		Line(double a, double b, double c, double d) : Shape(a,b),xEnd(c),yEnd(d) {} 

    	//get method for rectagular length
		double getxEnd() const{	return xEnd;}

		//get method for rectangular width
		double getyEnd() const{ return yEnd;}

		//overwrite draw method
		void draw(ostream& s){
			s << getxOffset() << ' ' << getyOffset() << " moveto\n";
			s << getxEnd()  << ' ' << getyEnd() << " lineto\n";
			s << "closepath\nstroke\n";
		}
};

class Polygon : public Shape{
	private:
		double radius;
		uint64_t numSides;

	public:
		//constructor
		Polygon(double a, double b, double c, double d) : Shape(a,b),radius(c),numSides(d) {} 

		//get method for rectagular length
		double getRadius() const{ return radius;}

		//get method for rectangular width
		double getSides() const{ return numSides;}

		//overwrite draw method
		void draw(ostream& s){
			double x = getxOffset(); 
			double y = getyOffset();
			s << x + (getRadius() * cos(0)) << ' ' << y + (getRadius() * sin(0)) << " moveto\n";
			for(int side = 1; side <= getSides(); side++){ //iterate through each side
				s << x + (getRadius() * cos(side*2*M_PI/getSides())) << ' ' << y + (getRadius() * sin(side*2*M_PI/getSides())) << " lineto\n";
			}
			s << "closepath\nstroke\n";
		}
};

//write to file class
class Drawing {
	private:
		ofstream f;
		vector<Shape*> shapes;	//contains shape objects
		vector<string> colors;	//contains RGB color strings
		vector<int> stop_color;	//contains locations to insert color strings when writing

	public:
		Drawing(string filename) : f(filename.c_str()), shapes() {}

		//add shape objects to vector
		void add( Shape* s ) { shapes.push_back(s);}

		//get method - how many shapes are there?
		uint64_t numShapes() const{	return shapes.size();}

		//get method - how many colors are there?
		uint64_t getColors() const{	return colors.size();}

		//generate RGB color string, store string & location in respective vectors
		void setrgb(uint16_t r,uint16_t g,uint16_t b){
			colors.push_back(to_string(r) + ' ' + to_string(g) + ' ' + to_string(b) + " setrgbcolor\n");
			stop_color.push_back(numShapes());
		}

		void draw() {
			uint64_t temp = 0; uint64_t numColors = getColors();
			for (int i = 0; i < numShapes(); i++){	//iterate through shape objects
				if(numColors == temp){numColors=0;}	//if no more colors, skip the elseif
				else if(i == stop_color[temp]){		//if we are at a color location,
					this->f << colors[temp++];			//write RGB string to file
				}
				shapes[i]->draw(f);					//write shape to file
			}
		}
};

int main() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////  Pretty Header
	textcolor('g');
	cout << "\n\n_________________________________________________________________________________\n\n";
	cout << "A Demonstration of Postscript Image Generation\n";
	cout << "Written by Stephen Opet III\n";
	cout << "https://github.com/stephen-opet\n\n\n";
	textcolor('w');

	ofstream f(  );
	Drawing d("image.ps");
	d.setrgb(1,0,0); // set drawing color to be bright red:  1 0 0 setrgbcolor
	d.add(new FilledRect(100.0, 150.0, 200.0, 50)); // x y moveto x y lineto ... fill
	d.add(new Rect(100.0, 150.0, 200.0, 50));       // x y moveto x y lineto ... stroke
	for (int x = 0; x < 600; x += 100)
		d.add(new FilledCircle(x,200,50.0)); // x y r 0 360 arc fill
	d.setrgb(0,1,0); // the rest are green

	d.add(new Circle(0,0, 100)); // 0 0 100 0 360 stroke
	d.add(new Line(400,500, 600,550));
	d.add(new Polygon(200,200, 50, 6));
	d.draw();

	endProgram(0);
}