// ----------------------------
// Heather Toomey
// CIT-260
// Problem W8dot2
// June 13, 2020
// ----------------------------

package Week08;

import java.util.Date;

public class TestDate {

    public static void main(String[] args) {
        // 1. Tells the user what the program does.
        System.out.print("\nThis program uses the Date class to display a set of dates and times.\n\n");

        // 2. Using the Date class (described in section 9.6.1 of the textbook) Create a Date object
        // and set it's elapsed time to 10000, 100000, 10000000, 100000000, 1000000000, 10000000000,
        // and 100000000000 and displays the data and time using the toString( ) method, respectively.
        // Create object of class date, call the object date, and reserve space as an obj
        java.util.Date date = new java.util.Date();

        long number = 1000;
        for (int i = 1; i <= 8; i++) {
            number *= 10;
            // Looks like the 3rd number is increased by 100 times, not 10 times
            if (i == 3) {
                number *= 10;
            }
            // Did you mean to leave off a zero on the last number?
            // We multiplied the 2nd to last number by 100 to match the example output
            if (i != 7) {
                date.setTime(number);
                System.out.println(date.toString());
            }
        }

        // 3. Display a goodbye message.
        System.out.println("Goodbye ...");
    }
}
