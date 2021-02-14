// ----------------------------
// Heather Toomey
// CIT-260
// Problem W5dot1
// May 23, 2020
// ----------------------------
package Week05;

public class Program_5_1 {

    public static void main(String[] args) {
	    // 1. Tells the user what the program does.
        System.out.print("This program uses a loop to calculate pounds per kilogram \n" +
                "(1 kilogram = 2.2 pounds) and displays a table.\n\n");

        // Labels the table
        System.out.print("kilograms\tpounds\n");
        System.out.print("---------\t------\n");

        // 2. Uses a loop to calculate pounds per kilogram (1 kilogram = 2.2 pounds) and display the table below.
        for (int kg = 1; kg < 16; kg += 2) {
            System.out.print(kg + "\t\t\t" + String.format("%.1f", kg*2.2) + "\n");
        }
        // 3. Outputs a goodbye message.
        System.out.print("\nGoodbye\n\n");

    }
}

