// ----------------------------
// Heather Toomey
// CIT-260
// Problem W5dot2
// May 23, 2020
// ----------------------------
package Week05;

public class Program_5_2 {

    public static void main(String[] args) {
        // Declare and initialize variables.
        int counter = 0;
        int divisor1 = 5;
        int divisor2 = 6;

        // 1. Tells the user what the program does.
        System.out.print("This program displays all of the numbers from 100 to 1000\n" +
                "that are divisible by both 5 and 6.\n\n");

        // 2. Uses a loop to display a table of all of the numbers from 100 to
        // 1000 that are divisible by both 5 and 6.  The numbers will be separated
        // by exactly one space and there will 10 numbers per line.

        for (int n = 100; n <= 1000; n++) {
            if (n % (divisor1 * divisor2) == 0) {
                System.out.print(n + " ");
                if (++counter % 10 == 0) {
                    System.out.print("\n");
                }
            }
        }

        // 3. Displays a goodbye message.
        System.out.print("\nGoodbye\n");
    }
}
