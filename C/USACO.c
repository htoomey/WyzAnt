/*
ID: Harold Toomey
TASK: USACO Automated Test Template
LANG: C
*/
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
    FILE *fin  = fopen ("test.in", "r");
    FILE *fout = fopen ("test.out", "w");
    int a, b;
    
    fscanf (fin, "%d %d", &a, &b);      /* the two input integers */
    fprintf (fout, "%d\n", a + b);
    return (0);
}
