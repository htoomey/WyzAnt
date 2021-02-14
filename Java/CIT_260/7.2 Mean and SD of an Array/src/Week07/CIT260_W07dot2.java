// ----------------------------
// Heather Toomey
// CIT-260
// Problem W7dot2
// June 6, 2020
// ----------------------------

package Week07;

import java.util.Scanner;
import java.lang.Math;

public class CIT260_W07dot2 {

    public static void main(String[] args) {
        // Create an array and initialize
        double array[] = new double[5];

        //1. Tell the user what the program does.
        System.out.print("This program computes the mean and the standard deviation for a set of numbers.\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        //2. Prompts the user to enter five numbers.
        // AND 3. Saves the user's input in an array of doubles.
        for (int i = 0; i < array.length; i++) {
            System.out.print("Please enter a number: ");
            array[i] = Float.parseFloat(keyboard.next());
        }

        // 4. Uses the methods given to compute the mean and the standard deviation for the numbers in the array.
        double avg = mean(array);
        double sD = deviation(array);

        // 5. Displays the mean and the standard deviation with 2 digits after the decimal point.
        System.out.print("\nThe mean of this set of numbers is " + String.format("%.2f", avg) + "\n");
        System.out.print("The standard deviation is " + String.format("%.2f", sD) + "\n");

        // 6. Displays a goodbye message.
        System.out.print("Goodbye ...\n");
    }


    // method that computes and returns the mean value for an array of doubles
    public static double mean(double[ ] x) {
        double mean = 0;
        double total = 0;

        for (int i = 0; i < x.length; i++) {
            total += x[i];
        }
        double avg = total / x.length;
        return avg;
    }


    // method that computes and returns the standard deviation
    public static double deviation(double[ ] x) {
        double sumOfSquares = 0;
        double avg = 0;

        // Calculate the Mean using the previous Method
        avg = mean(x);

        // Calculate the sum of squares
        for (int i = 0; i < x.length; i++) {
            sumOfSquares += Math.pow(x[i] - avg, 2.0);
        }

        // Calculate the Standard Deviation
        double sD = Math.sqrt((1.0 / (x.length - 1.0)) * sumOfSquares);
        return sD;
    }
}