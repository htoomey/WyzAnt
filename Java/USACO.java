/*
ID: Harold Toomey
TASK: USACO Automated Test Template
LANG: JAVA
*/

import java.io.*;
import java.util.*;

class USACO {
  public static void main (String [] args) throws IOException {
    // Input file name
    BufferedReader in = new BufferedReader(new FileReader("1.in"));
    //Scanner in = new Scanner(System.in));	// Read from keyboard

    // Output file name
    PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("1.out")));
    //PrintWriter out = new PrintWriter(System.out));	// Print to Console

    // Use StringTokenizer vs. readLine/split -- lots faster
    StringTokenizer st = new StringTokenizer(f.readLine());   // Get line, break into tokens
    int i1 = Integer.parseInt(st.nextToken());    // first integer
    int i2 = Integer.parseInt(st.nextToken());    // second integer
    out.println(i1 + i2);                         // output result
    out.close();                                  // close the output file
  }
}
