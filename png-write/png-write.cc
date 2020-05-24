/*
	A Demonstration of Writing a PNG via NANOSVG library
		https://github.com/memononen/nanosvg/tree/master/example
	Written by Stephen Opet III, https://github.com/stephen-opet
	Published May 24, 2020 [in SARS-COV-2 quarentine :) ]

	This program demonstrates the following:
		Dynamic memory allocation via new/delete
		copy & move constructor implementation on bitmap class
		Writing a PNG via the nnosvg library
		operator overload

	The program uses a comprehensive and straightforward library to write a proper PNG image file!!
	Doing so from scratch is difficult and ugly - using a bitmap w/ this library is easy and fun!

	Designed and tested on UbuntuLinux w/ g++ compiler
*/

using namespace std;	//seems to include swap()? used in operator=
#include <iostream>
#include <string>		//using strings duh
#include <cstring>		//memcpy()
#include <cmath>		//round() -> Bresenham's_line_algorithm
#define STB_IMAGE_WRITE_IMPLEMENTATION	//writing to png lib
#include "stb_image_write.h"

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

class DynamicBitmap {
	private:
		uint32_t w, h;
		uint32_t *p; 

	public: 

		//Constructor 												 
		DynamicBitmap(uint32_t width, uint32_t height, uint32_t defaultBG) : w(width), h(height), p(new uint32_t[width*height]){
			for(int k = 0; k < w*h; k++)	//iterate through p(width) array
					p[k] = defaultBG;		//set every pixel to default background color (black in our case)
		}//end constructor

		//Destructor
		~DynamicBitmap(){
			delete []p;  
		}//end destructor

		//copy constructor
		DynamicBitmap(const DynamicBitmap& orig) : w(orig.w), h(orig.h), p(new uint32_t[orig.w * orig.h]) {
			memcpy(p, orig.p, orig.w * orig.h * sizeof(uint32_t)); 
		}//end copy constructor

		//Operator =
		DynamicBitmap& operator =(DynamicBitmap copy){	// a2=a2
			w = copy.w;
			h = copy.h;
			swap(p, copy.p);
			return *this;
		}//end operator= 

		//move constructor
		DynamicBitmap(DynamicBitmap&& orig) : w(orig.w), h(orig.h), p(orig.p){
			orig.p = nullptr;
		}

		uint32_t &operator ()(uint32_t x, uint32_t y){
			cout << hex; //want to see hex value, not decimal
			return p[y*w + x];
		} //set 1 | this is triggered by both get and set, for some reason???

		//sets a pixel given coordinates & color
		//multiplication makes inefficient for simple shapes, but useful for complex algorithms
		void set(uint32_t x, uint32_t y, uint32_t color){
			p[y*w + x] = color;
		} //set

		void horizLine(uint32_t x1, uint32_t x2, uint32_t y, uint32_t color){
			uint32_t i = y*w+x1; 	//only uses multiplication once - more efficient than using set()
			for(; x1 <= x2; x1++)
				p[i++] = color;
		}

		void vertLine(uint32_t y1, uint32_t y2, uint32_t x, uint32_t color){
			uint32_t index = y1*w+x; 	//only uses multiplication once - more efficient than using set()
			for(uint32_t i = 0; i < (y2-y1); i++){
				p[index] = color;
				index+=w;
			}
		}

		//draws simple, hollow rectangle using horizontal & vertical line functions
		void drawRect(uint32_t x1, uint32_t y1, uint32_t rectW, uint32_t rectH, uint32_t color){
			horizLine(x1,x1+rectW,y1,color);
			vertLine(y1,y1+rectH,x1,color);
			horizLine(x1,x1+rectW,y1+rectH,color);
			vertLine(y1,y1+rectH,x1+rectW,color);
		}

		//uses horzontal line function to fill in a rectangle 
		void fillRect(uint32_t x1, uint32_t y1, uint32_t rectW, uint32_t rectH, uint32_t color){
			for(uint32_t i = 0; i<=rectH; i++)
				horizLine(x1, x1+rectW, y1+i, color);
		}

		//assuming arguments are given as x0,y0,x1,y1,color - this order was not explicit
		void line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
			int64_t slope = 2 * (x1 - x0); 			//slope
		   	int64_t epsilon = slope - (y1 - y0); 	//error
		   	for (int64_t y = y0, x = x0; y <= y1; y++) { 
		      set(x,y,color); 
		      epsilon += slope; 
		  	  if (epsilon >= 0){ 
		         x++; 
		         epsilon -= 2*(y1 - y0); 
		      } 
		    }
		}

		//draw an ellipse
		void ellipse(int xcenter, int ycenter, int xd, int yd, uint32_t color){ 
		    double rad_conversion = 3.14159265359 / 180;	//need conversion for C++ radian trig functions	  	

		    //compute angular eccentricity;
		    //must account for either x or y acting as either major/minor axis
		  	double a,b;
		  	if(yd < xd){
		  		b = yd/2;
		  		a = xd/2;
		  	}
		  	else{
		  		b = xd/2;
		  		a = yd/2;
		  	}
		  	double alpha = acos(b/a);

		  	//arbitrarily choose to draw four points per degree about center of ellipse 
		    for (double theta = 0; theta < 360; theta += 0.25) { 
		        int64_t x = a * cos(rad_conversion * theta) * cos(rad_conversion * alpha) + b * sin(rad_conversion * theta) * sin(rad_conversion * alpha); 
		        int64_t y = b * sin(rad_conversion * theta) * cos(rad_conversion * alpha) - a * cos(rad_conversion * theta) * sin(rad_conversion * alpha); 		  
		        set(xcenter+x,ycenter-y,color); 
		    } 
		} 

		//write the PNG
		void save(string filename) const{
			stbi_write_png(filename.c_str(), w, h, 4, p, w*4);
			cout << "\n\n\tData written to file, " + filename + "\n\n"; //formatting on main()
			textcolor('w');
		}
};

int main() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////  Pretty Header
	textcolor('g');
	cout << "\n\n_________________________________________________________________________________\n\n";
	cout << "A Demonstration of Writing a PNG via NANOSVG library\n";
	cout << "Written by Stephen Opet III\n";
	cout << "https://github.com/stephen-opet\n\n\n";
	textcolor('w');

	constexpr uint32_t BLACK = 	0xFF000000; // black opaque
	constexpr uint32_t RED 	=	0xFF0000FF; // red opaque
	constexpr uint32_t BLUE = 	0xFFFF0000;	// blue opaque
	constexpr uint32_t WHITE = 	0xFFFFFFFF;	// white opaque
	constexpr uint32_t YELLOW = 0xFF00FFFF;	// yellow opaque
	constexpr uint32_t GREEN = 	0xFF00FF00;	// green opaque
	
	int xcenter = 100;
	int ycenter = 100;
	int xdiameter = 200;
	int ydiameter = 100;

	DynamicBitmap b(1024, 1024, BLACK); // Potentially dynamic size (Now: 1024 x 1024 pixels)
	b(32,32) = RED;
	cout << b(32,32);
	
	b.horizLine(0, 500, 200, RED); // Red horizontal line, from x=0 to x=500, at y = 200
	b.vertLine(0, 399, 300, RED); // Red vertical line, from y=0 to y=399, at x = 300
	
	b.drawRect(200,200, 100,50, BLUE); // Blue rectangle, TOP-LEFT at x=200, y=200. width=100, height=50
	b.fillRect(201,201, 98,48, WHITE); // White rectangle, same rules as above, but filled with color
	
	b.line(400,0, 550,300, YELLOW); // Line drawn using https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
	
	b.ellipse(xcenter, ycenter, xdiameter, ydiameter, GREEN); //Ellipse using specs from above
	
	b.save("bitmap2.png"); //create the png file

	endProgram(0);
	
}