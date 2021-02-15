#include <time.h>
#include <sstream>
#include <stack>
#include <queue>
#include "LeafNodes.h"

int t0 = clock() , t1, TreeSize;
ifstream file;
ofstream fout; 


void count_chars( int *counts )
{
	for ( int i = 0 ; i <256 ; i++ )
		counts[ i ] = 0;
	if ( !file ) {
		cout <<"Couldn't open the input file!\n";
		throw "abort";
	}
	file.unsetf( ios::skipws );
	for ( ; ; ) {
		unsigned char c;
		file>> c;
		if ( file )
			counts[ c ]++;
		else
			break;
   }
}


int main()
{
	string filename;
	cout<<"Enter in the file to Compress:";
	cin>>filename;
	file.open(filename, ios::binary);
	cout<<"Enter in the decompresed file name:";
	cin>>filename;
	fout.open("Bites", ios::binary);
	int counts[ 256 ];
	count_chars( counts );
	priority_queue < node > q;
	node* child0;
	node* child1;

	stringstream s;
	unsigned long c;
	unsigned int v = 0;
	char d, e;
	unsigned int DataSize;
	// Creates the Binary Tree
	for ( int i = 0 ; i <256 ; i++ )
		if ( counts[ i ] )
			q.push( node( i, counts[ i ] ) );
	TreeSize = q.size();

	while ( q.size() >1 ) {
		child0 = new node( q.top() );
		q.pop();
		child1 = new node( q.top() );
		q.pop();
		q.push( node( child0, child1 ) );
		TreeSize += 1;
	}
	DataSize = file.tellg();
file.clear();
file.seekg(0);


/////////////////////////////////// Compression
	q.top().traverse();

	fout.write((char*)&TreeSize, 4);
	fout << q.top();
	fout.write((char*)&DataSize, 4);

	while(!file.eof()){
	c = file.get();
	child0 = q.top().find(c); // find node with char value
	if (!child0)
		continue;
	s << child0->code; // gets the code from the node and puts it into the stream
	v += child0->code.size();

	d = 0x00;
		if (v >=8) { // checks if the strings in the stream are bigger than 8 bits
			for (int i = 0; i < 8; i++){
				s >> e;
				d = d << 1;
				d = d | ((e == '0')? 0x00: 0x01);
			}
			fout << d;	// 0000 0100
			v -= 8;
		}
}

	file.close();
	fout.close();

////////////////////////////////////// Decompression	

	stack<node> ChildStack;
	file.open("Bites", ios::binary); // Compressed File
	fout.open(filename, ios::binary); // Decompressed File

	file.read((char*)&TreeSize, 4);
	char ReadBytes;

	for (int i =0; i <TreeSize; i++){
		file.read(&ReadBytes, 1);
		if (ReadBytes == '+' && file.peek() == '+') {
			file.read(&ReadBytes, 1);
			ChildStack.push(node(ReadBytes));
		}
		else if (ReadBytes == '+' && file.peek() == '#') {
			file.read(&ReadBytes, 1);
			child1 = new node( ChildStack.top() );
			ChildStack.pop();
			child0 = new node( ChildStack.top() );
			ChildStack.pop();
			ChildStack.push( node( child0, child1 ) );
		}
		else
			ChildStack.push(node(ReadBytes));
	}

	file.read((char*)&DataSize, 4);

	ChildStack.top().traverse();
	stringstream k;
	child0 = &ChildStack.top();

	while(!file.eof() && (fout.tellp() < DataSize)){
	file.read(&ReadBytes,1);
	for (int i = 0; i < 8; i++){
		d = 0x80 >> i;
		k << ((ReadBytes & d)? '1' : '0');
	}

	do {
		k >> e;
		if (k.eof())
			break;
		child0 = child0->walk(e);
		if (child0->value != 256) {

			fout << (char)child0->value;
			child0 = &ChildStack.top();
		}
	} while (!k.eof());
	k.clear();
}

/*
////// Frequency Table of Huffman Code
	cout <<"Character Frequency Charts" <<endl;
	cout<<endl;
	cout <<"Compressed:" << endl;
	cout << q.top() << endl;
	cout<<"File encoded.\n"<<endl;
	t1 = clock() - t0;
	cout<<"Execution Time:"<< t1<<"ms \n"<<endl;
	cout<<"Decompressed: "<<endl;
	cout << ChildStack.top() << endl;
	cout<<"File decoded.\n"<<endl;
	t1 = clock() - t0;
	cout<<"Execution Time:"<< t1<<"ms \n"<<endl;
	system("Pause");
*/	
	return 0;
}
