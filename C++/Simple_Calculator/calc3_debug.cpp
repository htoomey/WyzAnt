// ***************************************************
// Program: calc3_debug
// Author: Harold Toomey
// Date: 17 June 2020
//
// To Build:
//    % g++ -o calculator calc3_debug.cpp
//
// To Run:
//    % ./calculator_debug < formula.txt
//
// Test Input:
//	  5^;
//	  1000 + 6^ - 5^ + 1;
//
// Test Output:
//    25
//    1012
// ***************************************************

#include <iostream>
#include <string>
using namespace std;
#define DEBUG 1		// Set to 1 to see debug couts, otherwise set to 0

int main() {

	int sum = 0;			// Stores the partial sum and final sum
	int num = 0; 			// Stores a number from the formula
	char op = '\0'; 		// Stores an operator from the formula
	int caret_found;		// Set if we find a caret '^'
	bool semicolon_found = false;	// Set when we find the ';'
	string num_op_string;	// declare a string to store numbers and operators from formula.txt
	string::size_type sz;   // Needed for stoi().  alias of size_t

#if DEBUG
	cout << "==============================" << endl;
#endif
	while (cin >> num_op_string) {	

#if DEBUG
	cout << "num_op_string:  " << num_op_string << endl;
#endif	
	
		// Set num to zero if we added/subtracted it already in the switch statement below
		if (!op) num = 0;	
		
		// If the string is a number, then save it to num
		if (isdigit(num_op_string[0])) {
			num = stoi(num_op_string, &sz);
#if DEBUG
cout << "   num:  " << num << endl;
#endif		
		}

		if (num) {
			// If the string is a number, check if it has the '^' after it
			caret_found = num_op_string.find('^');
			// If no matches were found, the function returns string::npos
			if (caret_found != string::npos) {
				num = num * num;	// Match found, so square the number
#if DEBUG
cout << "   num^: " << num << endl;
#endif		
			}

			// Special Case: First number of the equation
			if (sum == 0) {
				sum = num;
			}
		}
		else {
			// If the string is not a number, then it must be an operator
			op = num_op_string[0];
#if DEBUG	
cout << "   op: " << op << endl;
#endif		
		}

		// Perform the operation.  The pattern is sum, operator, number.
		if (op && num) {
			switch (op) {
				case '+' : sum += num; break; 
				case '-' : sum -= num; break; 
				case ';' : semicolon_found = true;
						   cout << sum << endl << endl;
						   sum = 0;
						   break; 
				default : break; 
			}
			op = '\0';
#if DEBUG
cout << "   sum:  " << sum << endl;
#endif		
		}
		
		// Is this the last number or operand of the equation with no space before it?
		// This cout is for the final and last one 
		if ((!semicolon_found) && (num_op_string[num_op_string.length()-1] == ';')) {
			cout << sum << endl << endl;
			sum = 0;
			semicolon_found = false;
			op = '\0';  // NEW
		}

	}	// while()

#if DEBUG
	cout << "==============================" << endl;
#endif
	
	return 0; 
}
