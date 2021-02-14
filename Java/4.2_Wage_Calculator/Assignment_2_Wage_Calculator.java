package Week04;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
	// Tell the user ...
        System.out.print("Given your name, hours worked, " +
                "and hourly wage, this program calculates your \n" +
                "gross pay, state withholding tax, federal withholding tax, and your net pay.\n" +
                "It then displays your pay stub.\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        System.out.print("Enter your first and last name: ");
        String name = keyboard.next();
        name = name + " " + keyboard.next();

        System.out.print("Enter the hours you worked this week: ");
        double hours = Float.parseFloat(keyboard.next());

        System.out.print("Enter your hourly wage: ");
        double wage = Float.parseFloat(keyboard.next());

        double stateTaxMult = 0.09;
        double fedTaxMult = 0.20;

        double gross = wage * hours;
        double stateTax = gross * stateTaxMult;
        double fedTax = gross * fedTaxMult;
        double netPay = gross - stateTax - fedTax;

        // Print the pay stub
        System.out.print("\nPay Stub for " + name);
        System.out.print("\nHours Worked: " + hours);
        System.out.print("\nHourly Wage: $" + wage);
        System.out.print("\nGross Pay: $" + String.format("%.2f",gross));
        System.out.print("\nState Tax Withheld: $" + String.format("%.2f",stateTax));
        System.out.print("\nFederal Tax Withheld: $" + String.format("%.2f",fedTax));
        System.out.print("\nNet Pay: $" + String.format("%.2f",netPay));
        System.out.print("\n\nGoodbye.\n");
    }
}
