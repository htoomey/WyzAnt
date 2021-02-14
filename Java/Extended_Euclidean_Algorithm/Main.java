package EEA;

/**
 ** Java Program to implement the Extended Euclidean Algorithm
 **/

import java.util.Scanner;

public class Main
{
    public static void main (String[] args)
    {
        Scanner scan = new Scanner(System.in);
        System.out.println("Extended Euclidean Algorithm");

        /* Create an object from the ExtendedEuclidean class */
        ExtendedEuclidean eea = new ExtendedEuclidean();

        /* Prompt for two integers */
        System.out.println("Solves for the equation: ax + by = gcd(a, b)\n");
        System.out.println("Enter integers for a and b: ");
        long a = scan.nextLong();
        long b = scan.nextLong();

        System.out.println("a = " + a);
        System.out.println("b = " + b);

        /* Solve by calling the ExtendedEuclidean class */
        eea.ExtendedEuclideanFunction(a, b);

        System.out.println("\nGoodbye ...");
    }
}