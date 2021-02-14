// W03 Assignment: Program 3.2
package Week03;
import java.util.Scanner;

public class Program_3_2 {

    public static void main(String[] args) {

        // 1. Tells the user what the program does.
        System.out.print("Given a year and a month in that year, this program will tell you\n" +
                "the number of days in that month.\n");

        // Make scanner class object that accepts keyboard input
        Scanner keyboard = new Scanner(System.in);

        // 2. Prompts the user to enter a year.
        System.out.print("Enter a year: ");

        // 3. Gets the user's input and saves it.
        int year = Integer.parseInt(keyboard.next());

        // 4. Prompts the user to enter a value for the month(1 = Jan, 2 = Feb, etc).
        System.out.print("Enter a value for the month(1 = Jan, 2 = Feb, etc): ");

        // 5a. Gets the user's input and saves it.
        int month = Integer.parseInt(keyboard.next());

        // 5b. If the value is is not between 1 and 12 inclusive, tell the user and terminate the program.
        if (month < 1 || month > 12) {
            System.out.print("\n" + month + " is invalid. Month values must be between between 1 and 12, inclusive.");
            System.out.print("\nGoodbye.\n");
            System.exit(2);
        }

        // 6. Calculates the number of days in that month.
        // Your program should correctly handle leap years (see listing 3.7 in the textbook).
        String monthText = "";
        int numDaysInMonth = 0;
        switch (month) {
            case 1:  monthText = "January";   numDaysInMonth = 31; break;
            case 2:  monthText = "February";
                if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
                    numDaysInMonth = 29; else numDaysInMonth = 28; break;
            case 3:  monthText = "March";     numDaysInMonth = 31; break;
            case 4:  monthText = "April";     numDaysInMonth = 30; break;
            case 5:  monthText = "May";       numDaysInMonth = 31; break;
            case 6:  monthText = "June";      numDaysInMonth = 30; break;
            case 7:  monthText = "July";      numDaysInMonth = 31; break;
            case 8:  monthText = "August";    numDaysInMonth = 31; break;
            case 9:  monthText = "September"; numDaysInMonth = 30; break;
            case 10: monthText = "October";   numDaysInMonth = 31; break;
            case 11: monthText = "November";  numDaysInMonth = 30; break;
            case 12: monthText = "December";  numDaysInMonth = 31; break;
            default: break;
        }

        // 7. Displays the number of days in that month.
        System.out.print("\n" + monthText + " of " + year + " has " + numDaysInMonth + " days in it.");

        // 8. Outputs a goodbye message.
        System.out.print("\nGoodbye.\n");
    }
}
