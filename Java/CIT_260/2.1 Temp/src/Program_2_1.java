// W02 Assignment: Program 2.1
package Week02;
import java.util.Scanner;

public class Program_2_1 {

    public static void main(String[] args) {

        // 1. Tells the user what the program does.
        System.out.print("This program converts a temperature in degrees Celsius into\n" +
                "a temperature in degrees Fahrenheit. ");

        // 2. Prompts the user to enter a temperature in degrees Celsius.
        System.out.print("Enter a temperature in\n" + "degrees Celsius: ");

        // Make scanner class object that accepts keyboard input
        Scanner keyboard = new Scanner(System.in);

        // 3. Gets the user's input and saves it in a variable declared as a double.
        double degreesCelsius = Float.parseFloat(keyboard.next());

        // 4. Converts the Celsius temperature to a temperature in degrees Fahrenheit, using the formula:
        // temp = (9.0/5.0) x temp + 32
        double degreesFahrenheit = (9.0/5.0) * degreesCelsius + 32;

        // 5. Outputs the Celsius temperature and the Fahrenheit temperature, with 2 digits after the decimal point.
        System.out.print("\n" + String.format("%.2f", degreesCelsius) + " degrees Celsius is equal to " +
                        String.format("%.2f", degreesFahrenheit) + " degrees Fahrenheit.");

        // 6. Outputs a goodbye message.
        System.out.print("\nGoodbye.\n");
    }
}
