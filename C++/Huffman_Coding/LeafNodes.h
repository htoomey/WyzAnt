#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct node {
	int weight;
	unsigned short value;
	string code;
	node * const child0;
	node * const child1;


	// overloaded operator for reading in the files
	friend std::ostream& operator<<(std::ostream& o, node& n) {
		if (n.child0){
			o << *(n.child0);
			o << *(n.child1);
		}
		else {
			o << (char)n.value << ","  << n.weight << ":\t" << n.code << std::endl;
		}

		return o;
	}

	// overloaded operator for writing out the files
	friend std::ofstream& operator<<(std::ofstream& o, node& n) {
		if(n.child0){
			o << *(n.child0);
			o << *(n.child1);
			o << "+#";
	}
		else{
			if ((char)n.value =='+')
				o << '+';
			o.write((char*)&n.value, 1);
		}
		return o;
	}

	// default constructor
	node( unsigned char c = 0, int i = -1 ): child0(0), child1(0) {
		value = c;
		weight = i;
		code = "";
	}

	// contructor 
	node( node * const c0, node *const c1 ): child0(c0), child1(c1) {
		value = 256;
		weight = c0->weight + c1->weight;
		code = "";
	}

	// overloaded operaters for compression and decompression
	node& operator=(node& rhs) {
		const_cast<node*>(child0) = rhs.child0;
		const_cast<node*>(child1) = rhs.child1;
		value = rhs.value;
		weight = rhs.weight;
		code = rhs.code;
		return *this;
	}


	bool operator<( const node &a ) const {
		return weight >a.weight;
	}

	// Generate the codes
	void traverse(string generate="") {
	code.append(generate);
	if ( child0 ) {
		child0->traverse( code + '0' );
		child1->traverse( code + '1' );
	}

}

	// find for compression of the tree
	node* find(unsigned char c){
		if(value == c)
			return this;
		else{
			node* n = 0;
		   if(child0)
				n = child0->find(c);
		   if(!n && child1)
			   n = child1->find(c);
		   if(!n)
			   return 0;
		   return n;
		}
	}

	// walk for decompression of the tree
	node* walk(unsigned char c){
		if(c == '0')
			return child0;
		if(c == '1')
			return child1;
		return 0;
	}

};

