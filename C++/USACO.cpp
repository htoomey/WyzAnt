/*
ID: Harold Toomey
TASK: USACO Automated Test Template
LANG: C++
*/

/* LANG can be C++11 or C++14 for those more recent releases */
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ofstream fout ("test.out");
    ifstream fin ("test.in");
    int a, b;
    
    fin >> a >> b;
    fout << a + b << endl;
    return (0);
}
