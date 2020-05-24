/*
	A Demonstration of Generic Programming via a Templated List
	Written by Stephen Opet III, https://github.com/stephen-opet
	Published May 24, 2020 [in SARS-COV-2 quarentine :) ]

	This program uses a single custom templated list class
	to create, operate, and print lists of varying types
	Generic Programming allows us to reuse the same code for each implementation

	Designed and tested on UbuntuLinux w/ g++ compiler
*/

#include <iostream>
using namespace std;

template<typename T>
class GrowArray {
private:
	uint64_t size,capacity;
	T* p;
	void checkGrow(){ //no overhead
		if(size >= capacity){
			T* old  = p;
			p = new T[capacity = size + size]; //double it, quadratic; sets capacity to new value
			for(uint64_t i = 0; i < size; i++)
				p[i]=old[i];
			delete [] old; 
		}
	}
	void checkShrink(){ //shrinks array quadratically if >1/2 is empty
		if(capacity >= 2*size){
			T* old = p;
			p = new T[capacity = capacity / 2]; //halves capacity
			for(uint64_t i = 0; i < size; i++)
				p[i] = old[i];
			delete []old;
		}
	}

public:
	//Constructor 1: no argument
	GrowArray() : capacity(1), size(0), p(new T[1]){}
	//Constructor 2: default capacity
	GrowArray(uint64_t initCapacity) : p(new T[initCapacity]), size(0), capacity(initCapacity){}
	//Destructor
	~GrowArray() { delete []p; } 
	//Compy Constructor
	GrowArray(const GrowArray& orig) : capacity(orig.capacity), size(orig.size), p(new T[orig.capacity]) { 
		memcpy(p, orig.p, orig.size * sizeof(T));
	}
	//Operator= Member
	GrowArray& operator =(GrowArray copy){	
		capacity = copy.capacity;
		size = copy.size;
		swap(p, copy.p);
		return* this;
	}
	//Move Constructor
	GrowArray(GrowArray&& orig) : capacity(orig.capacity), size(orig.size), p(orig.p){
		orig.p = nullptr;
	}
	//add member - checks if array needs to grow, then adds argument
	void add(T v) {
		checkGrow();
		p[size++] = v;
	}
	//remove member - shrinks if more than half is unused
	void removeEnd(){
		checkShrink();
		size--;
	}
	friend ostream& operator <<(ostream& s, const GrowArray& d){
		for(int i = 0; i < d.size; i++){
			s << d.p[i] << " ";
		}
		return s;
	}
};

int main() {
	GrowArray<int> a(100);
	for (int i = 0; i < 1000000; i++)
		a.add(i);
	for (int i = 0; i < 999990; i++)
		a.removeEnd();
	// should have 0 1 2 3 4 5 6 7 8 9		 
	cout << a << '\n';
	
	GrowArray<string> b;	//List<string> b;
	b.add("hello");
	string s[] = { "test", "this", "now" };
	
	for (int i = 0; i < sizeof(s)/sizeof(string); i++)
		b.add(s[i]);
	cout << b << '\n';
	
}