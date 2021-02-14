// ----------------------------
// Heather Toomey
// CIT-260
// Problem W7dot1
// June 6, 2020
// ----------------------------

package Week07;

import java.util.Scanner;

public class CIT260_W07dot1 {

    public static void main(String[] args) {
        // Create an array and initialize
        double array[] = new double[5];

        //1. Tell the user what the program does.
        System.out.print("This program calculates the average of five numbers.\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        //2. Prompts the user to enter 5 double values.
        // AND 3. Stores these values in an array.
        for (int i = 0; i < array.length; i++) {
            System.out.print("Please enter a number: ");
            array[i] = Float.parseFloat(keyboard.next());
        }

        // 4. Calls the average method you wrote to calculate and return the average value of the numbers in the array.
        double avg = average(array);

        // 5. Displays the average value, with two digits after the decimal point.
        System.out.print("The average of these five numbers is " + String.format("%.2f", avg));

        // 6. Displays a goodbye message.
        System.out.print("\nGoodbye ...\n");
    }


    public static double average(double[ ] array) {
        // Declare var
        double total = 0;

        // Calculate average of array values
        for (int i = 0; i < array.length; i++){
            total += array[i];
        }
        double avg = total / array.length;
        return avg;
    }
}