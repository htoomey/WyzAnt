// ----------------------------
// Heather Toomey
// CIT-260
// Problem W6dot2
// May 30, 2020
// ----------------------------

package Week06;

public class CIT260_W6dot2 {

    public static void main(String[] args) {

        double fahrenheit;
        double celsius;

        //1. Tell the user what the program does.
        System.out.print("\nThis program converts " +
                "degrees Fahrenheit to degrees Celsius and vice versa.\n\n");

        System.out.print("Celsius\t\tFahrenheit\t|\tFahrenheit\t\tCelsius\n");
        System.out.print("----------------------------------------------------\n");

        //2.  Invoke the above two methods to compute and display the table below.
        for (int i = 10; i >= 1; i--) {
            celsius = 30.0 + i;
            System.out.print(String.format("%4.1f",celsius) + "\t\t");
            System.out.print(String.format("%5.1f",celsiusToFahrenheit(celsius))+ "\t\t|\t");

            fahrenheit = 120.0 - 10*(10-i);
            System.out.print(fahrenheit + "\t\t\t");
            System.out.print(String.format("%5.2f",fahrenheitToCelsius(fahrenheit)) + "\n");
        }

        // 3. Output a goodbye message.
        System.out.print("\nGoodbye.\n");
    }

    // Method #1 - C to F
    public static double celsiusToFahrenheit(double tempCelsius) {
        return (tempCelsius * (9.0 / 5.0) + 32.0);
    }

    // Method #2 - F to C
    public static double fahrenheitToCelsius(double tempFahrenheit) {
        return ((5.0 / 9.0) * (tempFahrenheit - 32.0));
    }

}