// W04 Assignment: Program 4.2
package Week04;
import java.util.Scanner;

public class Program_4_2 {

    public static void main(String[] args) {

        double stateTaxPercentage = 0.09;
        double fedTaxPercentage = 0.20;

        // 1. Tells the user what the program does.
        System.out.print("Given your name, hours worked, and hourly wage, this program calculates your \n" +
                "gross pay, state withholding tax, federal withholding tax, and your net pay.\n" +
                "It then displays your pay stub.\n\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        // 2. Prompts the user to enter their first and last name.
        System.out.print("Enter your first and last name: ");

        // 3. Gets the user's input and saves it as a String.
        String name = keyboard.next();
        name += " " + keyboard.next();

        // 4. Prompts the user to enter the number of hours they worked this week.
        System.out.print("Enter the hours you worked this week: ");

        // 5. Gets the user's input and saves it.
        double hours = Float.parseFloat(keyboard.next());

        // 6. Prompts the user to enter their hourly wage.
        System.out.print("Enter your hourly wage: ");

        // 7. Get's the user's input and saves it.
        double wage = Float.parseFloat(keyboard.next());

        // 8. Calculates the state withholding tax (9%) and the federal withholding tax (20%).
        double gross = wage * hours;
        double stateTax = gross * stateTaxPercentage;
        double fedTax = gross * fedTaxPercentage;

        // 9. Calculates your gross pay and your pay after subtracting the withholding taxes.
        double netPay = gross - stateTax - fedTax;

        // 10. Outputs a pay statement as shown in the example below.
        System.out.print("\nPay Stub for " + name);
        System.out.print("\nHours Worked: " + hours);
        System.out.print("\nHourly Wage: $" + String.format("%.2f", wage));
        System.out.print("\nGross Pay: $" + String.format("%.2f", gross));
        System.out.print("\nState Tax Withheld: $" + String.format("%.2f", stateTax));
        System.out.print("\nFederal Tax Withheld: $" + String.format("%.2f", fedTax));
        System.out.print("\nNet Pay: $" + String.format("%.2f", netPay));

        // 11. Outputs a goodbye message.
        System.out.print("\n\nGoodbye.\n");
    }
}
