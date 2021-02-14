package Week06;

import java.util.Scanner;

public class CIT260_W6dot1 {

    public static void main(String[] args) {
        // 1. Tells the user what the program does.
        System.out.print("\nGiven an investment amount and an annual interest rate, this program \n" +
                "will calculate the future value of the investment for a period of\n" +
                "ten years.\n\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);


        // 2. Prompts the user to an investment amount, for example, 1000.
        System.out.print("Enter a positive, non-zero value for the investment: ");

        // 3a. Gets the user's input and saves it.
        String investmentAmountString = keyboard.next();
        double investmentAmount = Double.parseDouble(investmentAmountString);

        // 3b. The user's input must be a positive, non-zero value.
        if (investmentAmount <= 0) {
            System.out.print("ERROR: Please enter a positive, non-zero value.\n\nGoodbye...\n\n");
            System.exit(1);
        }


        // 4. Prompts the user to enter an annual interest rate, for example, 9%.
        System.out.print("Enter an annual interest rate, between 0 and 100: ");

        // 5a. Gets the user's input and saves it.
        String annualInterestRateString = keyboard.next();
        double annualInterestRate = Double.parseDouble(annualInterestRateString);

        // 5b. The user's input must be positive and less than 100.
        if (annualInterestRate <= 0 || annualInterestRate >= 100) {
            System.out.print("ERROR: Please enter a value between 0 and 100.\n\nGoodbye...\n\n");
            System.exit(2);
        }


        // Labels the table
        System.out.print("\nYears\tFutureValue\n");

        // 6. Using a method that you have written, calculate and display the future value of the investment
        // for a period of 10 years. The formula for computing the future value of an investment is
        // futureValue = investmentAmount * (1 + monthlyInterestRate)^(numberOfYears * 12)
        double future;
        for (int years = 1; years <= 10; years++) {
            future = futureValue(investmentAmount,annualInterestRate / 1200, years);
            System.out.print(years + "\t\t" + "$" + String.format("%.2f", future) + "\n");
        }

        // Outputs a goodbye message.
        System.out.print("\nGoodbye ...\n");
        return;
    }

    // The formula for computing the future value of an investment is:
    //         futureValue = investmentAmount * (1 + monthlyInterestRate)^(numberOfYears * 12)
    public static double futureValue(double investmentAmount, double monthlyInterestRate, int years) {
        return (investmentAmount * Math.pow(1.0 + monthlyInterestRate, years * 12));
    }

}
