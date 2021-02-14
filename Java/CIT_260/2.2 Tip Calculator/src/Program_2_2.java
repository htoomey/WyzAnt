// W02 Assignment: Program 2.2
package Week02;
import java.util.Scanner;

public class Program_2_2 {

    public static void main(String[] args) {

        double taxPercentage = 3.2;

        // Tells the user what the program does.
        System.out.print("Given the price of a meal and a percentage to use for the tip,\n" +
                "this program calculates the tip, the tax, and the total amount of the bill.\n");

        // Make scanner class object that accepts keyboard input
        Scanner keyboard = new Scanner(System.in);

        // Prompts the user to enter a value for the cost of their meal.
        System.out.print("Enter the cost of the meal: ");

        // Gets the user's input and saves it in a variable declared as a double.
        double mealCost = Float.parseFloat(keyboard.next());

        // Prompts the user to enter a percentage value for the tip.
        System.out.print("Enter the tip percentage: ");

        // Gets the user's input and saves it in a variable declared as a double.
        double tipPercentage = Float.parseFloat(keyboard.next());

        // Calculates the value of the tip, using the percentage entered by the user.
        double tip = mealCost * tipPercentage/100.0;

        // Calculates the tax on the meal. The tax rate is 3.2%.
        double tax = mealCost * taxPercentage/100.0;

        // Calculates the total bill = meal cost + tip + tax.
        double totalBill = mealCost + tip + tax;

        // Outputs the tip, the tax, and the total value of the bill.
        // All values will be formatted to show a dollar sign and 2 digits after the decimal point.
        System.out.print("\nThe Tip is $" + String.format("%.2f", tip));
        System.out.print("\nThe Tax is $" + String.format("%.2f", tax));
        System.out.print("\nThe total bill is $" + String.format("%.2f", totalBill));

        // Outputs a goodbye message.
        System.out.print("\nGoodbye.\n");
    }
}
