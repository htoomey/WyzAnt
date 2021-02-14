// W03 Assignment: Program 3.1
package Week03;
import java.util.Scanner;

public class Program_3_1 {

    public static void main(String[] args) {

        // 1. Tells the user what the program does.
        System.out.print("Given today's day of the week and some number of days in the future\n" +
                "this program will tell you the day of the week for the future day.\n");

        // Make scanner class object that accepts keyboard input
        Scanner keyboard = new Scanner(System.in);

        // 2. Prompts the user to enter a value for today's day of the week (0 for Sunday, 1, for Monday, etc. ).
        System.out.print("Enter today's day of the week (0 for Sunday, 1 for Monday, etc): ");

        // 3a. Gets the user's input and saves it.
        int dayOfWeek = Integer.parseInt(keyboard.next());

        // 3b. If the value is less than 0 or greater than 6, tell the user the input is
        // invalid, and terminate the program.
        if (dayOfWeek < 0 || dayOfWeek > 6) {
            System.out.print("\n" + dayOfWeek + " is invalid. You must enter 0 - 6.\n");
            System.exit(2);
        }

        String dayOfWeekText = "";
        switch (dayOfWeek) {
            case 0: dayOfWeekText = "Sunday"; break;
            case 1: dayOfWeekText = "Monday"; break;
            case 2: dayOfWeekText = "Tuesday"; break;
            case 3: dayOfWeekText = "Wednesday"; break;
            case 4: dayOfWeekText = "Thursday"; break;
            case 5: dayOfWeekText = "Friday"; break;
            case 6: dayOfWeekText = "Saturday"; break;
            default: break;
        }

        // 4. Prompts the user to enter some number of days in the future. This can be any positive value.
        System.out.print("Enter the number of days in the future: ");

        // 5a. Gets the user's input and saves it.
        int futureDays = Integer.parseInt(keyboard.next());

        // 5b. If the value is negative, tell the user the input is invalid and terminate the program.
        if (futureDays < 0) {
            System.out.print("\n" + futureDays + " is invalid. You must enter a positive number.\n");
            System.exit(3);
        }

        // 6. Calculates the day of the week for the future day.
        int futureDayOfWeek = (dayOfWeek + futureDays) % 7;

        String futureDayOfWeekText = "";
        switch (futureDayOfWeek) {
            case 0: futureDayOfWeekText = "Sunday"; break;
            case 1: futureDayOfWeekText = "Monday"; break;
            case 2: futureDayOfWeekText = "Tuesday"; break;
            case 3: futureDayOfWeekText = "Wednesday"; break;
            case 4: futureDayOfWeekText = "Thursday"; break;
            case 5: futureDayOfWeekText = "Friday"; break;
            case 6: futureDayOfWeekText = "Saturday"; break;
            default: break;
        }

        // 7. Displays the day of the week for today and the day of the week for the future day.
        System.out.print("\nToday is " + dayOfWeekText + " and the future day is " + futureDayOfWeekText);

        // 8. Outputs a goodbye message.
        System.out.print("\nGoodbye.\n");
    }
}
