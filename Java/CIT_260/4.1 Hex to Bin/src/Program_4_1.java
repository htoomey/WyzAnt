// W04 Assignment: Program 4.1
package Week04;
import java.util.Scanner;

public class Program_4_1 {

    public static void main(String[] args) {

        // 1. Tells the user what the program does.
        System.out.print("\nThis program converts a hexadecimal digit into a four digit binary number.\n");

        // 2. Prompts the user to enter a hexadecimal digit.
        System.out.print("Enter a hexadecimal number: ");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        //Make variable for binary value
        String bin = "0000";

        // 3a. Gets the user's input and saves it.
        char hex = keyboard.next().charAt(0);

        // Convert character to uppercase
        hex = Character.toUpperCase(hex);

        // 3b. If the value is not a valid hexadecimal digit tell the user and terminate the program.
        if (hex < '0' || (hex > '9' && hex < 'A') || hex > 'F') {
            System.out.println(hex + " is not a valid hexadecimal digit.\nGoodbye.");
            System.exit(2);
        }

        // 4. Converts the hexadecimal digit into a four digit binary number.
        switch (hex) {
            case '0':  bin = "0000"; break;
            case '1':  bin = "0001"; break;
            case '2':  bin = "0010"; break;
            case '3':  bin = "0011"; break;
            case '4':  bin = "0100"; break;
            case '5':  bin = "0101"; break;
            case '6':  bin = "0110"; break;
            case '7':  bin = "0111"; break;
            case '8':  bin = "1000"; break;
            case '9':  bin = "1001"; break;
            case 'A':  bin = "1010"; break;
            case 'B':  bin = "1011"; break;
            case 'C':  bin = "1100"; break;
            case 'D':  bin = "1101"; break;
            case 'E':  bin = "1110"; break;
            case 'F':  bin = "1111"; break;
            default: break;
        }

        // 5. Outputs the four digit binary number, including any leading zeros.
        System.out.print("\nThe binary value is " + bin + ".");

        // 6. Outputs a goodbye message.
        System.out.print("\nGoodbye.\n");
    }
}
