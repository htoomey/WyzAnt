package Week04;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {


        System.out.print("\nThis program converts a " +
                "hexadecimal digit into a four digit " +
                "binary number. \nEnter a hexadecimal number: ");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        //Make variable for binary value
        String bin = "0001";

        // let user input hex value and press enter
        char hex = keyboard.next().charAt(0);

        // Convert character to uppercase
        hex = Character.toUpperCase(hex);

        if (hex < '0' || hex > 'F') {
            System.out.println(hex + " is not a valid hexadecimal digit.\nGoodbye.");
            System.exit(2);
        }

        // Convert hex to binary (table lookuo)
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

        // Print the value inputted
        System.out.print("\nThe binary value is " + bin + ".");
        System.out.print("\nGoodbye.\n");
    }
}
