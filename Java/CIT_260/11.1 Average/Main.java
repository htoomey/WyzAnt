// Heather Toomey
// CIT-260
// Assignment W10dot1
// June 27, 2020

package Week10;

import java.util.Scanner;   // import the Scanner class
import java.util.ArrayList; // import the ArrayList class

public class Main {

    public static void main(String[] args) {

        // Is used to determine how many triangles to create.  Assignment asks for 3.
        final int MAX_TRIANGLES = 3;

        // Format:
        // ArrayList<dataType> arrayName    = new ArrayList<dataType>();
           ArrayList<Triangle> triangleList = new ArrayList<>();

        //1. Tells the user what the program does.
        System.out.print("This program gets input for three triangles from the user.\n" +
                "It then creates three Triangle objects and displays the\n" +
                "description of each.\n");

        Scanner input = new Scanner(System.in);

        // 2. Prompts the user to enter the values for the sides, color, and filled attribute for three different
        // triangle objects.
        // 4. Using the values entered by the user, creates three Triangle objects and stores them in an
        // ArrayList<GeometricObject>.
        for (int i = 0; i < MAX_TRIANGLES; i++) {
            triangleList.add(new Triangle());       // Create a Triangle object and add it to the list

            // 2a. Prompts the user to enter the values for the color attribute for a triangle object.
            System.out.println("Enter the color of a triangle (e.g. \"red\"): ");
            triangleList.get(i).setColor(input.next());     // 3. Saves the users input.

            // 2b. Prompts the user to enter the values for the filled attribute for a triangle object.
            System.out.println("Is the triangle filled (y or n):");
            String filled = input.next();   // 3. Saves the users input.
            filled = filled.toLowerCase();  // Just in case CAPS LOCK is on
                 if (filled.charAt(0) == 'y') { triangleList.get(i).setFilled(true);  }
            else if (filled.charAt(0) == 'n') { triangleList.get(i).setFilled(false); }
            else {
                System.out.print("Error. Please say n or y. Thank you. \n");
                return;
            }

            // 2c.  Prompts the user to enter the sides for a triangle object.
            System.out.println("Enter the lengths of the three sides of the triangle:");
            triangleList.get(i).setSide1(input.nextDouble());   // 3. Saves the users input.
            triangleList.get(i).setSide2(input.nextDouble());   // 3. Saves the users input.
            triangleList.get(i).setSide3(input.nextDouble());   // 3. Saves the users input.
        }

        // 5. Displays the string representation of each Triangle object in the ArrayList, by calling its toString( )
        // method and its getArea() method. See the output example.
        for (Triangle i : triangleList) {
            System.out.println(i.toString());
        }

        // 6. Displays a goodbye message.
        System.out.println("\nGoodbye...");
    }
}
